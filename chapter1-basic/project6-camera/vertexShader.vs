#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoor;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec2 texCoord;

void main(){
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);
    texCoord = texCoor;
}