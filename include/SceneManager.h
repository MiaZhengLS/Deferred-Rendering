#pragma once

#include <vector>
#include <string>

#include "Model.h"
#include "Camera.hpp"

class SceneManager
{
public:
  SceneManager();
  ~SceneManager();
  void addObjects(std::vector<std::string> modelFilePaths);
  void addObject(std::string modelFilePath, std::string vsFilePath = "", std::string fsFilePath = "");
  const std::vector<Model *> getObjects();
  void updateAll();
  void renderAll(RenderMode renderMode);
  const std::vector<glm::vec3> &getLightPositions() const
  {
    return m_lightPositions;
  }
  const std::vector<glm::vec3> &getLightColors() const
  {
    return m_lightColors;
  }

  Camera &getCamera()
  {
    return m_mainCamera;
  }

  void addLights(int addNum);

private:
  void configureLights();
  void configureCamera();
  void randomizeLights();

private:
  std::vector<Model *> m_objects;
  // Lights
  int m_lightNum = 32;
  std::vector<glm::vec3> m_lightPositions;
  std::vector<glm::vec3> m_lightColors;
  // Camera
  Camera m_mainCamera;
};
