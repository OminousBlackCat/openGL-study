#version 330 core
struct Material{
    //物体的材质选项
    vec3 ambient; //物体反射环境光的颜色
    vec3 diffuse; //物体漫反射颜色
    vec3 specular;
    float shininess; //物体的镜面反射材质选项 越大镜面反射光圈越小(越像镜子、越金属)
};

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
    vec3 ambinet; //环境光颜色
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
    //聚光(手电筒)
    vec3 position; //手电筒位置
    vec3 direction; //手电筒方向
    float cutOff;
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