#version 330 core

layout(location=0)in vec3 position; 
layout(location=1)in vec2 texCoord;

out VS_OUT{
  vec2 v_texCoord;
} vs_out;


void main()
{
    vs_out.v_texCoord = texCoord;
    gl_Position = vec4(position, 1.0);
}