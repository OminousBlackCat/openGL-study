#pragma once
#ifndef MESH_H
#define MESH_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include"shader.h"

using namespace std;

struct Vertex
{
	glm::vec3 position;  // ��������(ģ������ϵ��
	glm::vec3 normal;    // ���㷨����
	glm::vec2 texCoor;   // �������������
};

struct Texture
{
	unsigned int id;  // ����Ψһ���
	string type;      // ��������(diffuse or specular)
};

class Mesh
{
public:
	vector<Vertex> vertices;		// ��������
	vector<unsigned int> indices;	// ��ͼ��������
	vector<Texture> textures;		// ��������
	Mesh(vector<Vertex> vs, vector<unsigned int>is, vector<Texture>ts) {
		this->vertices = vs; // ���
		this->indices = is;
		this->textures = ts;
		

		this->setupMesh();
	}

	void draw(Shader shader) {


	}
	

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh()
	{
		glGenVertexArrays(1, &this->VAO);  // ����VAO VBO ��EBO
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);  // ��VAO ׼������VBO
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  // ��vertices����VBO�������VAOΨһ�Ķ��㻺�����

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);  // ��indices����EBO�������VAO�Ļ���˳�����
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // ��������0, ����ֲ�����ϵ�µ�����
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);  // ��ͷ��ʼѰ��, ���ΪVertex�ṹ��Ĵ�С

		glEnableVertexAttribArray(1); // ��������1, ���㷨����
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));  // ��ͷ��ʼѰ��, ���ΪVertex�ṹ��Ĵ�С, ƫ����Ϊnormal���ڵ�λ��
	
		glEnableVertexAttribArray(2); // ��������2, �����Ӧ����������
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoor));  // ��ͷ��ʼѰ��, ���ΪVertex�ṹ��Ĵ�С, ƫ����ΪtexCoor���ڵ�λ��
	}


};



#endif