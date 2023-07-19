#pragma once

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else // This works for Mac
#include <SDL.h>
#endif

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>
#include <string>
#include "Common.h"
#include "SceneManager.h"

// Purpose:
// This class sets up a full graphics program using SDL
class SDLGraphicsProgram
{
public:
  static SDLGraphicsProgram &instance()
  {
    if (m_instance == nullptr)
    {
      m_instance = new SDLGraphicsProgram();
    }
    return *m_instance;
  }
  // Destructor
  ~SDLGraphicsProgram();
  // Setup OpenGL
  bool initGL();
  // Per frame update
  void update();
  // Renders shapes to the screen
  void render();
  // Loop that runs forever
  void loop();
  // Listen to input
  void input(SDL_Event &e);
  // Get Pointer to Window
  SDL_Window *getSDLWindow();
  // Helper Function to Query OpenGL information.
  void getOpenGLVersionInfo();

  void addNewModel(std::string modelFileName);
  void removeModel(int modelIdx);
  void outputSceneInformation();

  uint16_t getScreenWidth() const { return m_screenWidth; }
  uint16_t getScreenHeight() const { return m_screenHeight; }
  SceneManager &getSceneManager()
  {
    return m_sceneManager;
  }
  void setScreenSize(uint16_t w, uint16_t h);

  void loadModel();

private:
  // Constructor
  SDLGraphicsProgram();
  static SDLGraphicsProgram *m_instance;

  // Screen dimension constants
  uint16_t m_screenWidth;
  uint16_t m_screenHeight;
  // The window we'll be rendering to
  SDL_Window *m_window{nullptr};
  // OpenGL context
  SDL_GLContext m_openGLContext;
  // If the program is going to quit
  bool m_quit{false};
  SceneManager m_sceneManager;
  RenderMode m_renderMode{RenderMode::DeferredRenderingMode};
};
