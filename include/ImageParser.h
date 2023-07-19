#pragma once

#include <string>
#include "Common.h"
#include <stb/stb_image.h>
#include <memory>
class ImageParser
{
public:
  ImageParser() {}
  virtual ~ImageParser()
  {
    // stbi_image_free(m_pixelData);
  }
  virtual bool loadImageFile(std::string fileName);
  const uint8_t *getPixels() { return m_pixelData.get(); }
  const int getWidth() { return m_width; }
  const int getHeight() { return m_height; }
  const int getChannels() { return m_channel; }

protected:
  int m_width;
  int m_height;
  int m_channel;
  // To store the returned pointer stbi_load returns and
  // make sure the memory is freed when ImageParser is deallocated,
  // I need to use unique_ptr instead of a raw pointer.
  // With a raw pointer, the memory is not governed by ImageParser,
  // when I tried to deallocate it in destructor, memory access is considered illegal.
  // But with unique_ptr it doesn't complain about it.
  std::unique_ptr<uint8_t> m_pixelData{nullptr};
};
