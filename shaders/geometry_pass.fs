#version 330 core

// The textures we write geometry information to
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT{
	vec3 FragPos;
	vec2 v_texCoord;
	mat3 TBN;
} fs_in;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_SpecularMap;

// out vec4 FragColor;

void main()
{
  gPosition = fs_in.FragPos;
  vec3 normal = texture(u_NormalMap, fs_in.v_texCoord).rgb;
  gNormal = fs_in.TBN * normal;
  gAlbedoSpec.rgb = texture(u_DiffuseMap, fs_in.v_texCoord).rgb;
  gAlbedoSpec.a = texture(u_SpecularMap, fs_in.v_texCoord).r;

  // FragColor = gAlbedoSpec;
}