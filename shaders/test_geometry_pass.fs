#version 330 core

in VS_OUT{
	vec3 FragPos;
	vec2 v_texCoord;
	mat3 TBN;
} fs_in;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_SpecularMap;
uniform int renderMode;

out vec4 FragColor;

void main()
{
  if(renderMode == 1) {
    FragColor = vec4(fs_in.FragPos, 1.0);
  }
  else if(renderMode == 2){
    vec3 normal = texture(u_NormalMap, fs_in.v_texCoord).rgb;
    FragColor = vec4(fs_in.TBN * normal, 1.0);
  }
  else if(renderMode == 3) {
    vec3 albedo = texture(u_DiffuseMap, fs_in.v_texCoord).rgb;
    FragColor = vec4(albedo, 1.0);
  }
  else {
    float spec = texture(u_SpecularMap, fs_in.v_texCoord).r;
    FragColor = vec4(spec, spec, spec, 1.0);
  }
}