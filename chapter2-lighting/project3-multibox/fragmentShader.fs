#version 330 core
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 texCoord;
in vec3 outNormal;
in vec3 fragPos;

out vec4 color;
uniform Material material;
uniform Light light;
uniform sampler2D ourTexture;
uniform sampler2D specTexture;
uniform vec3 viewPos;

void main(){
    //ambient light config
    vec3 ambientLight = light.ambient * vec3(texture(ourTexture, vec2(texCoord.x, 1.0f - texCoord.y)).xyz);

    //diffuse light
    vec3 norm = normalize(outNormal);
    vec3 lightVec = normalize(light.position - fragPos);
    float diffuseCoe = max(dot(norm, lightVec), 0.0);
    vec3 diffuseLight = light.diffuse * (diffuseCoe * vec3(texture(ourTexture, vec2(texCoord.x, 1.0f - texCoord.y)).xyz));

    //specular light
    vec3 reflectVec = reflect(-lightVec, norm);
    vec3 viewVec = normalize(viewPos - fragPos);
    float specularCoe = pow(max(dot(reflectVec, viewVec), 0.0), material.shininess);
    //vec3 specularLight = light.specular * (specularCoe * material.specular);
    vec3 specularLight = light.specular * (specularCoe * vec3(texture(specTexture, vec2(texCoord.x, 1.0f - texCoord.y)).xyz));

    vec3 result = ambientLight + diffuseLight + specularLight;
    color = vec4(result, 1.0f);
}