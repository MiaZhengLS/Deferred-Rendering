#include "DeferredRendering.h"
#include "SDLGraphicsProgram.h"
#include "Camera.hpp"

DeferredRendering::DeferredRendering() {}

void DeferredRendering::createShaderProgram(std::string gVsFile, std::string gFsFile, std::string lVsFile, std::string lFsFile)
{
  std::string errorMsg;
  std::string vertexShader = readFileContent(gVsFile, errorMsg);
  std::string fragmentShader = readFileContent(gFsFile, errorMsg);
  m_shaderGeometry.createShaderProgram(vertexShader, fragmentShader);
  vertexShader = readFileContent(lVsFile, errorMsg);
  fragmentShader = readFileContent(lFsFile, errorMsg);
  m_shaderLight.createShaderProgram(vertexShader, fragmentShader);

  configureGBuffer();
  configureFullScreenQuad();
}

void DeferredRendering::configureGBuffer()
{
  int width = (float)SDLGraphicsProgram::instance().getScreenWidth();
  int height = SDLGraphicsProgram::instance().getScreenHeight();
  glGenFramebuffers(1, &m_gBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

  glGenTextures(1, &m_gBufferPosition);
  glBindTexture(GL_TEXTURE_2D, m_gBufferPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBufferPosition, 0);

  glGenTextures(1, &m_gBufferNormal);
  glBindTexture(GL_TEXTURE_2D, m_gBufferNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBufferNormal, 0);

  glGenTextures(1, &m_gBufferAlbedoSpec);
  glBindTexture(GL_TEXTURE_2D, m_gBufferAlbedoSpec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBufferAlbedoSpec, 0);

  uint32_t attachments[3] = {GL_COLOR_ATTACHMENT0,
                             GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);

  glGenRenderbuffers(1, &m_gBufferDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, m_gBufferDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gBufferDepth);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "Frame buffer isn't complete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Connect texture slot with shader sampler
  m_shaderLight.bind();
  m_shaderLight.setUniform1i("positionMap", 0);
  m_shaderLight.setUniform1i("normalMap", 1);
  m_shaderLight.setUniform1i("albedoSpecMap", 2);
}

void DeferredRendering::configureFullScreenQuad()
{
  float quadVertices[] = {
      // positions
      -1.0f,
      1.0f,
      0.0f,
      // texture Coords
      0.0f,
      1.0f,

      // positions
      -1.0f,
      -1.0f,
      0.0f,
      // texture Coords
      0.0f,
      0.0f,

      // positions
      1.0f,
      1.0f,
      0.0f,
      // texture Coords
      1.0f,
      1.0f,

      // positions
      1.0f,
      -1.0f,
      0.0f,
      // texture Coords
      1.0f,
      0.0f,
  };
  // setup plane VAO
  glGenVertexArrays(1, &m_quadVAO);
  glGenBuffers(1, &m_quadVBO);
  glBindVertexArray(m_quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
}

void DeferredRendering::renderGeometryPass(Geometry geometry, std::vector<Transform *> transforms, glm::mat4 perspectiveMatrix)
{
  m_shaderGeometry.bind();
  float screenWidth = (float)SDLGraphicsProgram::instance().getScreenWidth();
  float screenHeight = SDLGraphicsProgram::instance().getScreenHeight();
  glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaderGeometry.setUniformMatrix4fv("viewMatrix", &SDLGraphicsProgram::instance().getSceneManager().getCamera().getWorldToViewmatrix()[0][0]);
    m_shaderGeometry.setUniformMatrix4fv("projectionMatrix", &perspectiveMatrix[0][0]);
    m_shaderGeometry.setUniform1i("u_DiffuseMap", 0);
    m_shaderGeometry.setUniform1i("u_NormalMap", 1);
    m_shaderGeometry.setUniform1i("u_SpecularMap", 2);
    for (size_t i = 0; i < transforms.size(); i++)
    {
      m_shaderGeometry.setUniformMatrix4fv("modelMatrix", transforms[i]->getTransformMatrix());
      glDrawElements(GL_TRIANGLES,
                     geometry.getIndicesSize(),
                     GL_UNSIGNED_INT,
                     nullptr);
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::renderLightingPass(std::vector<glm::vec3> lightPositions, std::vector<glm::vec3> lightColors)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_shaderLight.bind();
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_gBufferPosition);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_gBufferNormal);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_gBufferAlbedoSpec);
  m_shaderLight.setUniform1i("lightActualNum", lightPositions.size());
  // send light relevant uniforms
  for (unsigned int i = 0; i < lightPositions.size(); i++)
  {
    m_shaderLight.setUniform3fv("lights[" + std::to_string(i) + "].pos", lightPositions[i]);
    m_shaderLight.setUniform3fv("lights[" + std::to_string(i) + "].color", lightColors[i]);
    // update attenuation parameters and calculate radius
    const float linear = 0.7f;
    const float quadratic = 1.8f;
    m_shaderLight.setUniform1f("lights[" + std::to_string(i) + "].linear", linear);
    m_shaderLight.setUniform1f("lights[" + std::to_string(i) + "].quadratic", quadratic);
  }

  m_shaderLight.setUniform3fv("viewPos", SDLGraphicsProgram::instance().getSceneManager().getCamera().getEyePosition());
  // Render fullscreen
  glBindVertexArray(m_quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}