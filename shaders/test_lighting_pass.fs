#version 330 core
out vec4 FragColor;

in VS_OUT{
  vec2 v_texCoord;
} fs_in;

void main()
{
  FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}