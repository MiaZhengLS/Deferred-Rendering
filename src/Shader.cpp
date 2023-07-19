#include <fstream>

#include "Shader.h"
#include "Common.h"

// Constructor
Shader::Shader() {}

// Destructor
Shader::~Shader()
{
  // Deallocate Program
  glDeleteProgram(m_shaderID);
}

// Use our shader
void Shader::bind() const
{
  glUseProgram(m_shaderID);
}

// Turns off our shader
void Shader::unbind() const
{
  glUseProgram(0);
}

void Shader::createShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{

  // Create a new program
  unsigned int program = glCreateProgram();
  // Compile our shaders
  unsigned int myVertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  unsigned int myFragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  // Link our program
  // These have been compiled already.
  glAttachShader(program, myVertexShader);
  glAttachShader(program, myFragmentShader);
  // Link our programs that have been 'attached'
  glLinkProgram(program);
  glValidateProgram(program);

  // Once the shaders have been linked in, we can delete them.
  glDetachShader(program, myVertexShader);
  glDetachShader(program, myFragmentShader);

  glDeleteShader(myVertexShader);
  glDeleteShader(myFragmentShader);

  if (!checkLinkStatus(program))
  {
    log("CreateShader", "ERROR, shader did not link! Were there compile errors in the shader?");
  }

  m_shaderID = program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
  // Compile our shaders
  // id is the type of shader (Vertex, fragment, etc.)
  unsigned int id;

  if (type == GL_VERTEX_SHADER)
  {
    id = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (type == GL_FRAGMENT_SHADER)
  {
    id = glCreateShader(GL_FRAGMENT_SHADER);
  }
  const char *src = source.c_str();
  // The source of our shader
  glShaderSource(id, 1, &src, nullptr);
  // Now compile our shader
  glCompileShader(id);

  // Retrieve the result of our compilation
  int result;
  // This code is returning any compilation errors that may have occurred!
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *errorMessages = new char[length]; // Could also use alloca here.
    glGetShaderInfoLog(id, length, &length, errorMessages);
    if (type == GL_VERTEX_SHADER)
    {
      log("CompileShader ERROR", "GL_VERTEX_SHADER compilation failed!");
      log("CompileShader ERROR", (const char *)errorMessages);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
      log("CompileShader ERROR", "GL_FRAGMENT_SHADER compilation failed!");
      log("CompileShader ERROR", (const char *)errorMessages);
    }
    // Reclaim our memory
    delete[] errorMessages;
    // Delete our broken shader
    glDeleteShader(id);
    return 0;
  }

  return id;
}

// Check to see if linking was successful
bool Shader::checkLinkStatus(GLuint programID)
{
  // Retrieve the result of our compilation
  int result;
  // This code is returning any Linker errors that may have occurred!
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
    char *errorMessages = new char[length]; // Could also use alloca here.
    glGetProgramInfoLog(programID, length, &length, errorMessages);
    // Reclaim our memory
    SDL_Log("ERROR in linking process\n");
    SDL_Log("%s\n", errorMessages);
    delete[] errorMessages;
    return false;
  }

  return true;
}

GLuint Shader::getID() const
{
  return m_shaderID;
}
