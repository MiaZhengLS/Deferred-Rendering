#include "Transform.h"

// By default, all transform matrices
// are also identity matrices
Transform::Transform()
{
  loadIdentity();
}

Transform::~Transform()
{
}

// Resets the model transform as the identity matrix.
void Transform::loadIdentity()
{
  m_modelTransformMatrix = glm::mat4(1.0f);
}

void Transform::translate(float x, float y, float z)
{
  // Here we apply our camera
  // This first transformation is applied to the object
  // This is the model transform matrix.
  // That is, 'how do I move our model'
  // Here we see I have translated the model -1.0f away from its original location.
  // We supply the first argument which is the matrix we want to apply
  // this transformation to (Our previous transformation matrix.
  m_modelTransformMatrix = glm::translate(m_modelTransformMatrix, glm::vec3(x, y, z));
}

void Transform::translate(glm::vec3 offset){
  m_modelTransformMatrix = glm::translate(m_modelTransformMatrix, offset);
}

void Transform::rotate(float radians, float x, float y, float z)
{
  m_modelTransformMatrix = glm::rotate(m_modelTransformMatrix, radians, glm::vec3(x, y, z));
}

void Transform::scale(float x, float y, float z)
{
  m_modelTransformMatrix = glm::scale(m_modelTransformMatrix, glm::vec3(x, y, z));
}

void Transform::scale(float uniformScaleFactor)
{
  m_modelTransformMatrix = glm::scale(m_modelTransformMatrix, glm::vec3(uniformScaleFactor, uniformScaleFactor, uniformScaleFactor));
}

// Returns the actual transform matrix
// Useful for sending
GLfloat *Transform::getTransformMatrix()
{
  return &m_modelTransformMatrix[0][0];
}

// Get the raw internal matrix from the class
glm::mat4 Transform::getInternalMatrix() const
{
  return m_modelTransformMatrix;
}

void Transform::applyTransform(Transform t)
{
  m_modelTransformMatrix = t.getInternalMatrix();
}

// Perform a matrix multiplication with our Transform
Transform &Transform::operator*=(const Transform &t)
{
  m_modelTransformMatrix = m_modelTransformMatrix * t.getInternalMatrix();
  return *this;
}

// Perform a matrix addition with our Transform
Transform &Transform::operator+=(const Transform &t)
{
  m_modelTransformMatrix = m_modelTransformMatrix + t.getInternalMatrix();
  return *this;
}

// Matrix assignment
Transform &Transform::operator=(const Transform &t)
{
  m_modelTransformMatrix = t.getInternalMatrix();
  return *this;
}

// TODO: Do some more testing here!
// This code probably needs optimized (lots of new objects on the stack!)
Transform operator*(const Transform &lhs, const Transform &rhs)
{
  Transform result;

  result.m_modelTransformMatrix = lhs.getInternalMatrix() * rhs.getInternalMatrix();

  return result;
}

// TODO: Do some more testing here!
// This code probably needs optimized (lots of new objects on the stack!)
Transform operator+(const Transform &lhs, const Transform &rhs)
{
  Transform result;

  result.m_modelTransformMatrix = lhs.getInternalMatrix() + rhs.getInternalMatrix();

  return result;
}
