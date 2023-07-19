/** @file Camera.hpp
 *  @brief Sets up an OpenGL camera.
 *
 *  Sets up an OpenGL Camera. The camera is what
 *  sets up our 'view' matrix.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class Camera
{
public:
  Camera();
  // Return a 'view' matrix with our
  // camera transformation applied.
  glm::mat4 getWorldToViewmatrix() const;
  // Move the camera around
  void mouseLook(int mouseX, int mouseY);
  void moveForward(float speed);
  void moveBackward(float speed);
  void moveLeft(float speed);
  void moveRight(float speed);
  void moveUp(float speed);
  void moveDown(float speed);
  // Set the position for the camera
  void setCameraEyePosition(float x, float y, float z);
  // Set the view direction for the camera
  void setCameraViewDirection(float x, float y, float z);
  // Returns the Camera X Position where the eye is
  float getEyeXPosition();
  // Returns the Camera Y Position where the eye is
  float getEyeYPosition();
  // Returns the Camera Z Position where the eye is
  float getEyeZPosition();
  glm::vec3 getEyePosition();
  // Returns the X 'view' direction
  float getViewXDirection();
  // Returns the Y 'view' direction
  float getViewYDirection();
  // Returns the Z 'view' direction
  float getViewZDirection();

private:
  // Track the old mouse position
  glm::vec2 m_oldMousePosition;
  // Where is our camera positioned
  glm::vec3 m_eyePosition;
  // What direction is the camera looking
  glm::vec3 m_viewDirection;
  // Which direction is 'up' in our world
  // Generally this is constant, but if you wanted
  // to 'rock' or 'rattle' the camera you might play
  // with modifying this value.
  glm::vec3 m_upVector;
};

#endif
