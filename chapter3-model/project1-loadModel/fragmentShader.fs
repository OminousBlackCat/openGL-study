#version 330 core

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirctionalLight{
    //平行光(定向光)
    //无需光源位置 (光源无限远)
    vec3 direction; //光的方向 三维向量
    vec3 ambient; //环境光颜色
    vec3 diffuse; //漫反射颜色
    vec3 specular; //镜面反射颜色;
};

struct PointLight{
    //点光源
    //需要光源位置 并且光会沿着距离衰减(强度)
    vec3 position; //点光源位置
    vec3 ambient;  //环境光颜色
    vec3 diffuse;  //漫反射颜色
    vec3 specular; //镜面反射颜色
    float attenuation_constant;  //衰减常数项
    float attenuation_linear;    //衰减线性项
    float attenuation_quadratic; //衰减二次项
};

struct FlashLight{
    vec3 ambient;  //环境光颜色
    vec3 diffuse;  //漫反射颜色
    vec3 specular; //镜面反射颜色
    //聚光(手电筒)
    vec3 position; //手电筒位置
    vec3 direction; //手电筒方向
    float cutOff; //手电筒半切角的余弦值
    float outerCutOff; //外圆锥余弦值
    float attenuation_constant;  //衰减常数项
    float attenuation_linear;    //衰减线性项
    float attenuation_quadratic; //衰减二次项
};

in vec2 texCoord;
in vec3 outNormal;
in vec3 fragPos;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform PointLight pointLight;
uniform FlashLight flashLight;
uniform vec3 viewPos;

void main(){
    //ambient light config
    vec3 ambientLight = flashLight.ambient * vec3(texture(texture_diffuse1, texCoord).xyz);

    //diffuse light
    vec3 norm = normalize(outNormal);
    vec3 lightVec = normalize(flashLight.position - fragPos);
    float diffuseCoe = max(dot(norm, lightVec), 0.0);
    vec3 diffuseLight = flashLight.diffuse * (diffuseCoe * vec3(texture(texture_diffuse1, texCoord).xyz));

    //specular light
    vec3 reflectVec = reflect(-lightVec, norm);
    vec3 viewVec = normalize(viewPos - fragPos);
    float specularCoe = max(dot(reflectVec, viewVec), 0.0);
    //vec3 specularLight = light.specular * (specularCoe * material.specular);
    vec3 specularLight = flashLight.specular * (specularCoe * vec3(texture(texture_specular1, texCoord).xyz));

    //pointLight cut_off
    float theta = dot(lightVec, normalize(-flashLight.direction));
    float epsilon = flashLight.cutOff - flashLight.outerCutOff;
    float cut = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(flashLight.position - fragPos);
    float attenuation = 1.0f / (flashLight.attenuation_constant + flashLight.attenuation_linear * distance + flashLight.attenuation_quadratic * (distance * distance));

    vec3 result = ambientLight + cut * diffuseLight + cut * specularLight;
    color = vec4(result, 1.0f);
}