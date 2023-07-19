#include "ImageParser.h"

bool ImageParser::loadImageFile(std::string fileName)
{
  m_pixelData = std::unique_ptr<uint8_t>(stbi_load(fileName.c_str(), &m_width, &m_height, &m_channel, 0));
  return true;
}
