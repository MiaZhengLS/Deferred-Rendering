#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

void Camera::mouseLook(int mouseX, int mouseY)
{
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);
    // Detect how much the mouse has moved since
    // the last time
    glm::vec2 mouseDelta = 0.01f * (newMousePosition - m_oldMousePosition);

    m_viewDirection = glm::mat3(glm::rotate(-mouseDelta.x, m_upVector)) * m_viewDirection;

    // Update our old position after we have made changes
    m_oldMousePosition = newMousePosition;
}

void Camera::moveForward(float speed)
{
    m_eyePosition += m_viewDirection * speed;
}

void Camera::moveBackward(float speed)
{
    m_eyePosition -= m_viewDirection * speed;
}

void Camera::moveLeft(float speed)
{
    m_eyePosition += glm::mat3(glm::rotate(90.0f, m_upVector)) * m_viewDirection * speed;
}

void Camera::moveRight(float speed)
{
    m_eyePosition -= glm::mat3(glm::rotate(90.0f, m_upVector)) * m_viewDirection * speed;
}

void Camera::moveUp(float speed)
{
    m_eyePosition.y += speed;
}

void Camera::moveDown(float speed)
{
    m_eyePosition.y -= speed;
}

// Set the position for the camera
void Camera::setCameraEyePosition(float x, float y, float z)
{
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

void Camera::setCameraViewDirection(float x, float y, float z)
{
    m_viewDirection = glm::normalize(glm::vec3(x, y, z));
}

float Camera::getEyeXPosition()
{
    return m_eyePosition.x;
}

float Camera::getEyeYPosition()
{
    return m_eyePosition.y;
}

float Camera::getEyeZPosition()
{
    return m_eyePosition.z;
}

glm::vec3 Camera::getEyePosition()
{
    return m_eyePosition;
}

float Camera::getViewXDirection()
{
    return m_viewDirection.x;
}

float Camera::getViewYDirection()
{
    return m_viewDirection.y;
}

float Camera::getViewZDirection()
{
    return m_viewDirection.z;
}

Camera::Camera()
{
    // Position us at the origin.
    m_eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    // Looking down along the z-axis initially.
    // Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    // For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getWorldToViewmatrix() const
{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt(m_eyePosition,
                       m_eyePosition + m_viewDirection,
                       m_upVector);
}
