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
	glm::vec3 position;  // 顶点坐标(模型坐标系下
	glm::vec3 normal;    // 顶点法向量
	glm::vec2 texCoor;   // 顶点的纹理坐标
};

struct Texture
{
	unsigned int id;  // 纹理唯一编号
	string type;      // 纹理类型(diffuse or specular)
};

class Mesh
{
public:
	vector<Vertex> vertices;		// 顶点数组
	vector<unsigned int> indices;	// 绘图索引数组
	vector<Texture> textures;		// 纹理数组
	Mesh(vector<Vertex> vs, vector<unsigned int>is, vector<Texture>ts) {
		this->vertices = vs; // 深拷贝
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
		glGenVertexArrays(1, &this->VAO);  // 生成VAO VBO 与EBO
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);  // 绑定VAO 准备输入VBO
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  // 将vertices输入VBO当做这个VAO唯一的顶点缓存对象

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);  // 将indices输入EBO当做这个VAO的绘制顺序对象
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // 设置属性0, 顶点局部坐标系下的坐标
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);  // 从头开始寻找, 跨度为Vertex结构体的大小

		glEnableVertexAttribArray(1); // 设置属性1, 顶点法向量
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));  // 从头开始寻找, 跨度为Vertex结构体的大小, 偏移量为normal所在的位置
	
		glEnableVertexAttribArray(2); // 设置属性2, 顶点对应的纹理坐标
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoor));  // 从头开始寻找, 跨度为Vertex结构体的大小, 偏移量为texCoor所在的位置
	}


};



#endif