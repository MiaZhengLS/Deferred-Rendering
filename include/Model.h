#pragma once

#include "ModelParser.h"
#include "ObjModelParser.h"
#include "Texture.h"
#include "Geometry.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Transform.h"
#include "DeferredRendering.h"
#include "GeometryTestRendering.h"
#include "LightBulbRendering.h"
#include "RenderMode.h"

class Model
{
public:
  Model();
  ~Model();

  bool loadModel(const std::string modelFileName);
  void update();
  void render(RenderMode renderMode);

private:
  void configureRendering();
  void gBufferBind();
  void gBufferUnbind();

private:
  DeferredRendering deferredRendering;
  GeometryTestRendering gTestRendering;
  LightBulbRendering lightBulbRendering;
  
  glm::mat4 perspectiveMatrix;
  Texture m_diffuseMap;
  Texture m_normalMap;
  Texture m_specularMap;
  Geometry m_geometry;
  VertexBufferLayout m_vertexBufferLayout;
  Transform m_transform;
  std::vector<Transform*> m_modelTransforms;
  std::vector<glm::vec3> m_modelPositions;

  // gBuffer
  GLuint m_gBuffer;
  GLuint m_gBufferPosition;
  GLuint m_gBufferNormal;
  GLuint m_gBufferAlbedoSpec;
  GLuint m_gBufferDepth;
  GLuint m_quadVAO;
  GLuint m_quadVBO;
};