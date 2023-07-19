#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Common.h"
#include "Geometry.h"
#include "Transform.h"

class DeferredRendering
{
public:
  DeferredRendering();
  void createShaderProgram(std::string gVsFile, std::string gFsFile, std::string lVsFile, std::string lFsFile);
  void renderGeometryPass(Geometry geometry, std::vector<Transform*> transforms, glm::mat4 perspectiveMatrix);
  void renderLightingPass(std::vector<glm::vec3> lightPositions, std::vector<glm::vec3> lightColors);
  GLuint &getGBuffer() { return m_gBuffer; }

private:
  void configureGBuffer();
  void configureFullScreenQuad();

private:
  Shader m_shaderGeometry;
  Shader m_shaderLight;

  // gBuffer
  GLuint m_gBuffer;
  GLuint m_gBufferPosition;
  GLuint m_gBufferNormal;
  GLuint m_gBufferAlbedoSpec;
  GLuint m_gBufferDepth;
  GLuint m_quadVAO;
  GLuint m_quadVBO;
};