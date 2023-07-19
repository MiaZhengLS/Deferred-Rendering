#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else // This works for Mac
#include <SDL.h>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
  // Shader constructor
  Shader();
  // Shader Destructor
  ~Shader();
  // Use this shader in our pipeline.
  void bind() const;
  // Remove shader from our pipeline
  void unbind() const;
  // Create a Shader from a loaded vertex and fragment shader
  void createShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
  // return the shader id
  GLuint getID() const;
  // Set our uniforms for our shader.
  void setUniformMatrix4fv(const std::string name, const GLfloat *value) const
  {
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, value);
  }
  void setUniform3f(const std::string name, float v0, float v1, float v2) const
  {
    glUniform3f(glGetUniformLocation(m_shaderID, name.c_str()), v0, v1, v2);
  }
  void setUniform1i(const std::string name, int value) const
  {
    glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
  }
  void setUniform1f(const std::string name, float value) const
  {
    glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value);
  }
  void setUniform3fv(const std::string name, const glm::vec3 &value) const
  {
    glUniform3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, &value[0]);
  }

private:
  // Compiles loaded shaders
  unsigned int compileShader(unsigned int type, const std::string &source);
  // Makes sure shaders 'linked' successfully
  bool checkLinkStatus(GLuint programID);
  // Shader loading utility programs
  void printProgramLog(GLuint program);
  void printShaderLog(GLuint shader);
  // The unique shaderID
  GLuint m_shaderID;
};

#endif
