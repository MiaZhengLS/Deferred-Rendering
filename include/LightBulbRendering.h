#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Common.h"
#include "Shader.h"

class LightBulbRendering
{

public:
  LightBulbRendering();
  void createShaderProgram(std::string gVsFile, std::string gFsFile);
  void render(glm::mat4 perspectiveMatrix, std::vector<glm::vec3> lightPositions, std::vector<glm::vec3> lightColors);

private:
  void configureCube();
  void renderCube();

private:
  Shader m_shader;
  GLuint m_cubeVAO;
  GLuint m_cubeVBO;
};
