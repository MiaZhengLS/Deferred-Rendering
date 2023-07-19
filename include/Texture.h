#pragma once

#include <string>
#include <glad/glad.h>

#include "ImageParser.h"

class Texture
{
public:
  Texture();
  ~Texture();
  void loadTextureFile(const std::string textureFileName);
  void bind(uint8_t slot = 0);
  void unbind();

private:
  GLuint m_textureId;
  ImageParser m_imgParser;
};
