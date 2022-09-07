#version 330 core
in vec2 texCoord;

out vec4 color;
uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor; 
    color = texture(ourTexture, vec2(texCoord.x, 1.0f - texCoord.y)) * vec4(result, 1.0f);
}