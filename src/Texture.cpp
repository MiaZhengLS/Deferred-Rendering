#include <glad/glad.h>
#include <iostream>
#include "Texture.h"

Texture::Texture(/* args */)
{
}

Texture::~Texture()
{
}

void Texture::loadTextureFile(const std::string textureFileName)
{
  if (!m_imgParser.loadImageFile(textureFileName))
  {
    return;
  }
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &m_textureId);
  glBindTexture(GL_TEXTURE_2D, m_textureId);
  // filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // wrap mode
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GLenum format;
  int colorChannelNum = m_imgParser.getChannels();
  if (colorChannelNum == 1)
  {
    format = GL_RED;
  }
  else if (colorChannelNum == 3)
  {
    format = GL_RGB;
  }
  else if (colorChannelNum == 4)
  {
    format = GL_RGBA;
  }
  glTexImage2D(GL_TEXTURE_2D,
               0,
               format,
               m_imgParser.getWidth(),
               m_imgParser.getHeight(),
               0,
               format,
               GL_UNSIGNED_BYTE,
               m_imgParser.getPixels());


  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(uint8_t slotId)
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0 + slotId);
  glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::unbind()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
