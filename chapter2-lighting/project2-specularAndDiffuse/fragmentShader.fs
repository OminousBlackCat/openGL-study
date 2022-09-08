#version 330 core
in vec2 texCoord;
in vec3 outNormal;
in vec3 fragPos;

out vec4 color;
uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    float ambientStrength = 0.0f;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseCoe = 1.0f;
    vec3 norm = normalize(outNormal);
    vec3 lightVec = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightVec), 0.0);
    vec3 diffuse = diffuseCoe * diff * lightColor;

    float specCoe = 0.9f;
    vec3 viewVec = normalize(viewPos - fragPos);
    vec3 reflectVec = reflect(-lightVec, norm);
    float spec = pow(max(dot(viewVec, reflectVec), 0.0f), 32);
    vec3 specular = specCoe * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor; 
    color = texture(ourTexture, vec2(texCoord.x, 1.0f - texCoord.y)) * vec4(result, 1.0f);
}