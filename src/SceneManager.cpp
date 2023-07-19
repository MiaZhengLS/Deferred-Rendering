#include "SceneManager.h"

SceneManager::SceneManager()
{
  configureLights();
  configureCamera();
}

SceneManager::~SceneManager()
{
  for (size_t i = 0; i < m_objects.size(); i++)
  {
    delete m_objects[i];
  }
}

void SceneManager::configureLights()
{
  randomizeLights();
}

void SceneManager::addLights(int addNum)
{
  m_lightNum = std::min(64, std::max(m_lightNum + addNum, 0));
  randomizeLights();
}

void SceneManager::randomizeLights()
{
  srand(13);
  m_lightPositions.clear();
  m_lightColors.clear();
  for (unsigned int i = 0; i < m_lightNum; i++)
  {
    // calculate slightly random offsets
    float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
    float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
    float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 6.0);
    m_lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
    // std::cout << "Light " << i << ": " << xPos << "," << yPos << "," << zPos << std::endl;
    // also calculate random color
    float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
    float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
    float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
    m_lightColors.push_back(glm::vec3(rColor, gColor, bColor));
  }
}

void SceneManager::configureCamera()
{
  m_mainCamera.setCameraEyePosition(-3.0f, 2.0f, 2.0f);
  m_mainCamera.setCameraViewDirection(1.0f, -.5f, -1.5f);
}

void SceneManager::updateAll()
{
  for (size_t i = 0; i < m_objects.size(); i++)
  {
    m_objects[i]->update();
  }
}

void SceneManager::renderAll(RenderMode renderMode)
{
  for (size_t i = 0; i < m_objects.size(); i++)
  {
    m_objects[i]->render(renderMode);
  }
}

void SceneManager::addObjects(std::vector<std::string> modelFilePaths)
{
}

void SceneManager::addObject(std::string modelFilePath, std::string vsFilePath, std::string fsFilePath)
{
  Model *objPtr = new Model();
  if (!objPtr->loadModel(modelFilePath))
  {
    delete objPtr;
  }
  m_objects.push_back(objPtr);
}

const std::vector<Model *> SceneManager::getObjects()
{
  return m_objects;
}