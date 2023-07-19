#version 330 core
out vec4 FragColor;

in VS_OUT{
  vec2 v_texCoord;
} fs_in;

struct Light {
  vec3 pos;
  vec3 color;
  float linear;
  float quadratic;
};

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D albedoSpecMap;

// uniform vec3 lightPos; // Our light source position from where light is hitting this object
const int NR_LIGHTS = 64;
uniform Light lights[NR_LIGHTS];
uniform int lightActualNum;
uniform vec3 viewPos;  // Where our camera is
void main()
{
  vec3 ambientColor = vec3(0.1, 0.1, 0.1);
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  float specularIntensity = 1.0;

  vec3 albedo = texture(albedoSpecMap, fs_in.v_texCoord).rgb;

  vec3 fragPos = texture(positionMap, fs_in.v_texCoord).rgb;
  vec3 normal = texture(normalMap, fs_in.v_texCoord).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  float specularShininess = texture(albedoSpecMap, fs_in.v_texCoord).a;
  
  vec3 viewDir = normalize(viewPos - fragPos);

  vec3 lighting = ambientColor;
  for(int i = 0; i < lightActualNum; ++i)
  {
    vec3 lightDir = normalize(lights[i].pos - fragPos);

    // diffuse
    vec3 diffuseColor = max(0.0, dot(normal, lightDir)) * lights[i].color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), specularShininess);
    vec3 specularColor = specularIntensity * spec * lights[i].color;
    // attenuation
    float distance = length(lights[i].pos - fragPos);
    float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);
    diffuseColor *= attenuation;
    specularColor *= attenuation;
    lighting += diffuseColor + specularColor;
  }
  FragColor = vec4(lighting * albedo, 1.0);
}