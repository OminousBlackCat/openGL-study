#version 330 core
in vec2 texCoord;

out vec4 color;
uniform vec4 currentColor;
uniform sampler2D ourTexture;

void main(){
    color = texture(ourTexture, vec2(texCoord.x, 1.0f - texCoord.y)) * currentColor;
}