#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoor;
uniform float x_axis_offset;

out vec2 texCoord;

void main(){
    gl_Position = vec4(position.x + x_axis_offset, position.y, position.z, 1.0);
    texCoord = texCoor;
}