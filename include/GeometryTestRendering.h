#pragma once

#include <glm/glm.hpp>
#include "Common.h"
#include "Geometry.h"
#include "Transform.h"
#include "Shader.h"
#include "RenderMode.h"

class GeometryTestRendering
{
public:
  void createShaderProgram(std::string gVsFile, std::string gFsFile);
  void renderGeometryPass(Geometry geometry, std::vector<Transform *> transforms, glm::mat4 perspectiveMatrix, RenderMode gRenderMode);

private:
  Shader m_shaderGeometry;
};