#include "GeometryTestRendering.h"
#include "SDLGraphicsProgram.h"
#include "Camera.hpp"

void GeometryTestRendering::createShaderProgram(std::string gVsFile, std::string gFsFile)
{
  std::string errorMsg;
  std::string vertexShader = readFileContent(gVsFile, errorMsg);
  std::string fragmentShader = readFileContent(gFsFile, errorMsg);
  m_shaderGeometry.createShaderProgram(vertexShader, fragmentShader);
}

void GeometryTestRendering::renderGeometryPass(Geometry geometry, std::vector<Transform *> transforms, glm::mat4 perspectiveMatrix, RenderMode gRenderMode)
{
  m_shaderGeometry.bind();
  float screenWidth = (float)SDLGraphicsProgram::instance().getScreenWidth();
  float screenHeight = SDLGraphicsProgram::instance().getScreenHeight();
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderGeometry.setUniformMatrix4fv("viewMatrix", &SDLGraphicsProgram::instance().getSceneManager().getCamera().getWorldToViewmatrix()[0][0]);
    m_shaderGeometry.setUniformMatrix4fv("projectionMatrix", &perspectiveMatrix[0][0]);
    m_shaderGeometry.setUniform1i("u_DiffuseMap", 0);
    m_shaderGeometry.setUniform1i("u_NormalMap", 1);
    m_shaderGeometry.setUniform1i("u_SpecularMap", 2);
    m_shaderGeometry.setUniform1i("renderMode", gRenderMode);
    for (size_t i = 0; i < transforms.size(); i++)
    {
      m_shaderGeometry.setUniformMatrix4fv("modelMatrix", transforms[i]->getTransformMatrix());
      glDrawElements(GL_TRIANGLES,
                     geometry.getIndicesSize(),
                     GL_UNSIGNED_INT,
                     nullptr);
    }
  }
}