/** @file Geometry.hpp
 *  @brief Organizes vertex and triangle information.
 *
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#pragma once

#include <vector>

// Purpose of this class is to store vertice and triangle information
class Geometry
{
public:
  // Constructor
  Geometry();
  // Destructor
  ~Geometry();

  // Functions for working with individual vertices
  unsigned int getBufferSizeInBytes();
  // Retrieve the
  unsigned int getBufferDataSize();
  // Retrieve the Buffer Data Size
  float *getBufferDataPtr();
  // Add a new vertex
  void addVertex(float x, float y, float z, float s, float t, float nx = 0.0f, float ny = 0.0f, float nz = 1.0f);
  // gen pushes all attributes into a single vector
  void gen();
  // Functions for working with Indices
  // Creates a triangle from 3 indices
  // When a triangle is made, the tangents and bi-tangents are also
  // computed
  void makeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2);
  // Retrieve how many indices there are
  unsigned int getIndicesSize();
  // Retrieve the pointer to the indices
  unsigned int *getIndicesDataPtr();

private:
  // m_bufferData stores all of the vertexPositons, coordinates, normals, etc.
  // This is all of the information that should be sent to the vertex Buffer Object
  std::vector<float> m_bufferData;

  // Individual components of
  std::vector<float> m_vertexPositions;
  std::vector<float> m_textureCoords;
  std::vector<float> m_normals;
  std::vector<float> m_tangents;
  std::vector<float> m_biTangents;

  // The indices for a indexed-triangle mesh
  std::vector<unsigned int> m_indices;
};
