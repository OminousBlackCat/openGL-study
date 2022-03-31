#version 330 core
layout(location = 0) in vec3 position;
uniform float x_axis_offset;

void main(){
    gl_Position = vec4(position.x + x_axis_offset, position.y, position.z, 1.0);
}