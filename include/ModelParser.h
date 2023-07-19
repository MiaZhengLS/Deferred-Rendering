#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include "Common.h"

class ModelParser
{
public:
  ModelParser() {}
  virtual ~ModelParser() {}
  virtual bool parseModel(std::string modelFileName) { return true; }
  const std::string getErrorMsg() { return errorMsgStream.str(); }
  const std::vector<glm::vec3> &getVertices() { return vertices; }
  const std::vector<glm::vec3> &getNormals() { return normals; }
  const std::vector<glm::vec2> &getUVs() { return uvs; }
  const std::vector<uint32_t> &getVertexIndices() { return vertexIndices; }
  const std::vector<uint32_t> &getNormalIndices() { return normalIndices; }
  const std::vector<uint32_t> &getUVIndices() { return uvIndices; }
  virtual const std::string getDiffuseMapFileName() { return std::string(); }
  virtual const std::string getNormalMapFileName() { return std::string(); }
  virtual const std::string getSpecularMapFileName() { return std::string(); }

protected:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;
  std::vector<uint32_t> vertexIndices;
  std::vector<uint32_t> uvIndices;
  std::vector<uint32_t> normalIndices;
  std::stringstream errorMsgStream;
};