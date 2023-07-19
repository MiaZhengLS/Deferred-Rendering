#include <iostream>
#include <sstream>
#include <fstream>

#include "SDLGraphicsProgram.h"
#include "Camera.hpp"

SDLGraphicsProgram *SDLGraphicsProgram::m_instance = nullptr;

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram() : m_quit(false)
{
}

void SDLGraphicsProgram::setScreenSize(uint16_t w, uint16_t h)
{
  m_screenWidth = w;
  m_screenHeight = h;

  // Initialization flag
  bool success = true;
  // String to hold any errors that occur.
  std::stringstream errorStream;
  // The window we'll be rendering to
  m_window = NULL;
  // Render flag

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
    success = false;
  }
  else
  {
    // Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    m_window = SDL_CreateWindow("Deferred Rendering",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth,
                                m_screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Check if Window did not create.
    if (m_window == NULL)
    {
      errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
      success = false;
    }

    // Create an OpenGL Graphics Context
    m_openGLContext = SDL_GL_CreateContext(m_window);
    if (m_openGLContext == NULL)
    {
      errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
      success = false;
    }

    // Initialize GLAD Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
      errorStream << "Failed to iniitalize GLAD\n";
      success = false;
    }

    // Initialize OpenGL
    if (!initGL())
    {
      errorStream << "Unable to initialize OpenGL!\n";
      success = false;
    }
  }

  // If initialization did not work, then print out a list of errors in the constructor.
  if (!success)
  {
    errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
    std::string errors = errorStream.str();
    SDL_Log("%s\n", errors.c_str());
  }
  else
  {
    SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
  }

  // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
  getOpenGLVersionInfo();
}

void SDLGraphicsProgram::loadModel()
{
  m_sceneManager.addObject("resources/backpack/backpack.obj");
}

// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram()
{
  // Destroy window
  SDL_DestroyWindow(m_window);
  // Point m_window to NULL to ensure it points to nothing.
  m_window = nullptr;
  // Quit SDL subsystems
  SDL_Quit();
}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::initGL()
{
  // Success flag
  bool success = true;

  return success;
}

float cameraSpeed = 0.05f;
void SDLGraphicsProgram::input(SDL_Event &e)
{
  // Handle events on queue
  while (SDL_PollEvent(&e) != 0)
  {
    // User posts an event to quit
    // An example is hitting the "x" in the corner of the window.
    if (e.type == SDL_QUIT)
    {
      m_quit = true;
    }
    // Handle keyboard input for the camera class
    if (e.type == SDL_MOUSEMOTION)
    {
      // Handle mouse movements
      int mouseX = e.motion.x;
      int mouseY = e.motion.y;
      m_sceneManager.getCamera().mouseLook(mouseX, mouseY);
    }
    switch (e.type)
    {
    // Handle keyboard presses
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym)
      {
      case SDLK_LEFT:
        m_sceneManager.getCamera().moveLeft(cameraSpeed);
        break;
      case SDLK_RIGHT:
        m_sceneManager.getCamera().moveRight(cameraSpeed);
        break;
      case SDLK_UP:
        m_sceneManager.getCamera().moveForward(cameraSpeed);
        break;
      case SDLK_DOWN:
        m_sceneManager.getCamera().moveBackward(cameraSpeed);
        break;
      case SDLK_RSHIFT:
        m_sceneManager.getCamera().moveUp(cameraSpeed);
        break;
      case SDLK_RCTRL:
        m_sceneManager.getCamera().moveDown(cameraSpeed);
        break;
      case SDLK_q:
        m_quit = true;
        break;
      case SDLK_w:
        m_renderMode = RenderMode::WireFrame;
        break;
      case SDLK_0:
        m_renderMode = RenderMode::DeferredRenderingMode;
        break;
      case SDLK_1:
        m_renderMode = RenderMode::Position;
        break;
      case SDLK_2:
        m_renderMode = RenderMode::Normal;
        break;
      case SDLK_3:
        m_renderMode = RenderMode::Albedo;
        break;
      case SDLK_4:
        m_renderMode = RenderMode::Specular;
        break;
      case SDLK_PERIOD:
        m_sceneManager.addLights(5);
        break;
      case SDLK_MINUS:
        m_sceneManager.addLights(-5);
        break;
      }
      break;
    }
  } // End SDL_PollEvent loop.
}

// Update OpenGL
void SDLGraphicsProgram::update()
{
  m_sceneManager.updateAll();
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::render()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glViewport(0, 0, m_screenWidth, m_screenHeight);
  glClearColor(0.0f, 0.0f, 0.0f, 1.f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  if (m_renderMode == RenderMode::WireFrame)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  m_sceneManager.renderAll(m_renderMode);

  SDL_Delay(50);
}

// Loops forever!
void SDLGraphicsProgram::loop()
{
  SDL_Event e;
  // Enable text input
  SDL_StartTextInput();

  // While application is running
  while (!m_quit)
  {
    input(e);
    update();
    render();

    SDL_GL_SwapWindow(getSDLWindow());
  }
  // Disable text input
  SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window *SDLGraphicsProgram::getSDLWindow()
{
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::getOpenGLVersionInfo()
{
  SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
  SDL_Log("Vendor: %s", (const char *)glGetString(GL_VENDOR));
  SDL_Log("Renderer: %s", (const char *)glGetString(GL_RENDERER));
  SDL_Log("Version: %s", (const char *)glGetString(GL_VERSION));
  SDL_Log("Shading language: %s", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
