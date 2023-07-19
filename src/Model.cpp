#include <iostream>
#include <glm/glm.hpp>

#include "Model.h"
#include "Common.h"
#include "SDLGraphicsProgram.h"
#include "Camera.hpp"

Model::Model()
{
}

Model::~Model()
{
  std::cout << "model destruct" << std::endl;
}

bool Model::loadModel(const std::string modelFileName)
{
  ObjModelParser modelParser;
  if (!modelParser.parseModel(modelFileName))
  {
    std::cout << modelParser.getErrorMsg() << std::endl;
    return false;
  }
  const std::vector<glm::vec3> &vertices = modelParser.getVertices();
  const std::vector<glm::vec3> &normals = modelParser.getNormals();
  const std::vector<glm::vec2> &uvs = modelParser.getUVs();
  const std::vector<unsigned int> &vertexIndices = modelParser.getVertexIndices();
  const std::vector<unsigned int> &normalIndices = modelParser.getNormalIndices();
  const std::vector<unsigned int> &uvIndices = modelParser.getUVIndices();
  std::cout << vertices.size() << "," << normals.size() << "," << uvs.size() << std::endl;
  for (size_t i = 0; i < vertexIndices.size(); i++)
  {
    glm::vec3 vertex = vertices[vertexIndices[i]];
    glm::vec2 uv = (uvIndices.size() > 0 && uvIndices[i] < uvs.size()) ? uvs[uvIndices[i]] : glm::vec2(0.0f, 0.0f);
    glm::vec3 normal = (normalIndices.size() > 0 && normalIndices[i] < normals.size()) ? normals[normalIndices[i]] : glm::vec3(0.0f, 0.0f, 1.0f);
    m_geometry.addVertex(vertex.x, vertex.y, vertex.z, uv.s, uv.t, normal.x, normal.y, normal.z);
  }
  for (size_t i = 0; i < vertexIndices.size(); i += 3)
  {
    m_geometry.makeTriangle(i, i + 1, i + 2);
  }
  m_geometry.gen();
  m_vertexBufferLayout.createNormalBufferLayout(m_geometry.getBufferDataSize(), m_geometry.getIndicesSize(), m_geometry.getBufferDataPtr(), m_geometry.getIndicesDataPtr());

  // Load texture
  m_diffuseMap.loadTextureFile(modelParser.getDiffuseMapFileName());
  m_normalMap.loadTextureFile(modelParser.getNormalMapFileName());
  m_specularMap.loadTextureFile(modelParser.getSpecularMapFileName());

  
  int width = SDLGraphicsProgram::instance().getScreenWidth();
  int height = SDLGraphicsProgram::instance().getScreenHeight();
  perspectiveMatrix = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f);
 
  float startX = -4.0f;
  float startZ = -4.0f;
  float gapX = 3.5f;
  float gapZ = 2.5f;
  for (size_t i = 0; i < 5; i++)
  {
    for (size_t j = 0; j < 6; j++)
    {
      m_modelPositions.push_back(glm::vec3(startX + i * gapX, 0.0f, startZ - j * gapZ));
    }
  }
  for (size_t i = 0; i < m_modelPositions.size(); i++)
  {
    m_modelTransforms.push_back(new Transform());
  }

  configureRendering();
 
  return true;
}

void Model::configureRendering()
{
  deferredRendering.createShaderProgram("shaders/geometry_pass.vs", "shaders/geometry_pass.fs", "shaders/lighting_pass.vs", "shaders/lighting_pass.fs");
  gTestRendering.createShaderProgram("shaders/geometry_pass.vs", "shaders/test_geometry_pass.fs");
  lightBulbRendering.createShaderProgram("shaders/vert.vs", "shaders/frag.fs");
}

void Model::update()
{
  for (size_t i = 0; i < m_modelTransforms.size(); i++)
  {
    m_modelTransforms[i]->loadIdentity();
    m_modelTransforms[i]->scale(.5f);
    m_modelTransforms[i]->translate(m_modelPositions[i]);
  }
}

void Model::render(RenderMode renderMode)
{
  gBufferBind();
  if (renderMode == RenderMode::DeferredRenderingMode)
  {
    deferredRendering.renderGeometryPass(m_geometry, m_modelTransforms, perspectiveMatrix);
    deferredRendering.renderLightingPass(SDLGraphicsProgram::instance().getSceneManager().getLightPositions(), SDLGraphicsProgram::instance().getSceneManager().getLightColors());
  }
  else
  {
    gTestRendering.renderGeometryPass(m_geometry, m_modelTransforms, perspectiveMatrix, renderMode);
  }
  int width = SDLGraphicsProgram::instance().getScreenWidth();
  int height = SDLGraphicsProgram::instance().getScreenHeight();
  glBindFramebuffer(GL_READ_FRAMEBUFFER, deferredRendering.getGBuffer());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  lightBulbRendering.render(perspectiveMatrix, SDLGraphicsProgram::instance().getSceneManager().getLightPositions(), SDLGraphicsProgram::instance().getSceneManager().getLightColors());
}

void Model::gBufferBind()
{
  m_vertexBufferLayout.bind();
  m_diffuseMap.bind(0);
  m_normalMap.bind(1);
  m_specularMap.bind(2);
}