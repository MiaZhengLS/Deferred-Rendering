#include <iostream>
#include <fstream>
#include "ObjModelParser.h"

bool ObjModelParser::parseModel(std::string modelFileName)
{
  errorMsgStream.clear();
  std::ifstream file(modelFileName);
  if (file.is_open())
  {
    std::stringstream stream;
    stream << file.rdbuf();
    std::string token;
    float num[3];

    // stringstream >> can read one block of char at a time
    while (stream >> token)
    {
      // Vertex
      if (token == "v")
      {
        // Position: x, y, z
        for (size_t i = 0; i < 3; i++)
        {
          if (stream >> token)
          {
            try
            {
              num[i] = std::stof(token);
            }
            catch (std::invalid_argument &e)
            {
              errorMsgStream << e.what() << ": v " << token << " can't be converted to float" << std::endl;
              handleFileClose(file);
              return false;
            }
          }
          else
          {
            break;
          }
        }
        glm::vec3 vertex(num[0], num[1], num[2]);
        vertices.push_back(vertex);
      }
      // Normal
      else if (token == "vn")
      {
        // Normal vector: x, y, z
        for (size_t i = 0; i < 3; i++)
        {
          if (stream >> token)
          {
            try
            {
              num[i] = std::stof(token);
            }
            catch (std::invalid_argument &e)
            {
              errorMsgStream << e.what() << ": vn " << token << " can't be converted to float" << std::endl;
              handleFileClose(file);
              return false;
            }
          }
          else
          {
            break;
          }
        }
        glm::vec3 normal(num[0], num[1], num[2]);
        normals.push_back(normal);
      }
      // For files with texture uv
      else if (token == "vt")
      {
        // uv just needs 2 components
        for (size_t i = 0; i < 2; i++)
        {
          if (stream >> token)
          {
            try
            {
              num[i] = std::stof(token);
            }
            catch (std::invalid_argument &e)
            {
              errorMsgStream << e.what() << ": vt " << token << " can't be converted to float" << std::endl;
              handleFileClose(file);
              return false;
            }
          }
          else
          {
            break;
          }
        }
        glm::vec2 uv(num[0], num[1]);
        uvs.push_back(uv);
      }
      // Here we suppose the faces only contain triangles, so only 3 parts for 3 vertices.
      // For each vertex, it might one of these cases:
      // f v1 v2 v3               -- only vertex position index
      // f v1/vt1 ...             -- vertex position index / uv position index
      // f v1/vt1/vn1 ...         -- vertex position index / uv position index / normal vector index
      // f v1//vn1 ...            -- vertex position index / / normal vector index
      // Also notice index starts from 1 instead of 0.
      else if (token == "f")
      {
        for (size_t i = 0; i < 3; i++)
        {
          if (stream >> token)
          {
            size_t startIdx = 0;
            int tmpIdx = 0;
            num[1] = 0;
            num[2] = 0;
            for (size_t j = 0; j < token.length(); j++)
            {
              if (token[j] == '/')
              {
                if (j > startIdx)
                {
                  try
                  {
                    num[tmpIdx] = std::stoi(token.substr(startIdx, j - startIdx));
                  }
                  catch (std::invalid_argument &e)
                  {
                    errorMsgStream << e.what() << ": f " << token << " can't be converted to integer, j=" << j << ", startIdx=" << startIdx << std::endl;
                    handleFileClose(file);
                    return false;
                  }
                }
                else
                {
                  num[tmpIdx] = 0;
                }
                startIdx = j + 1;
                tmpIdx++;
              }
            }
            try
            {
              num[tmpIdx] = std::stoi(token.substr(startIdx, token.length() - startIdx));
            }
            catch (std::invalid_argument &e)
            {
              errorMsgStream << e.what() << ": f " << token << " can't be converted to integer, startIdx=" << startIdx << std::endl;
              handleFileClose(file);
              return false;
            }
            vertexIndices.push_back(num[0] - 1);
            if (num[1] > 0)
            {
              uvIndices.push_back(num[1] - 1);
              // std::cout << "uv index:" << num[1] << std::endl;
            }
            if (num[2] > 0)
            {
              normalIndices.push_back(num[2] - 1);
            }
          }
          else
          {
            break;
          }
        }
      }
      else if (token == "mtllib")
      {
        if (stream >> token)
        {
          std::string dir;
          std::string::size_type lastSlashIndex = modelFileName.rfind('/');
          if (lastSlashIndex != std::string::npos)
          {
            dir = modelFileName.substr(0, lastSlashIndex + 1);
          }
          std::string mtlFilePath = dir + token;
          std::ifstream mtlFile(mtlFilePath);
          if (mtlFile.is_open())
          {
            std::stringstream mtlFileStream;
            mtlFileStream << mtlFile.rdbuf();
            std::string mtlFiletoken;
            while (mtlFileStream >> mtlFiletoken)
            {
              if (mtlFiletoken == "map_Kd")
              {
                if (mtlFileStream >> mtlFiletoken)
                {
                  m_diffuseMapFileName = dir + mtlFiletoken;
                }
              }
              else if (mtlFiletoken == "map_Bump")
              {
                if (mtlFileStream >> mtlFiletoken)
                {
                  m_normalMapFileName = dir + mtlFiletoken;
                }
              }
              else if (mtlFiletoken == "map_Ks")
              {
                if (mtlFileStream >> mtlFiletoken)
                {
                  m_specularMapFileName = dir + mtlFiletoken;
                }
              }
            }
          }
        }
      }
    }
  }
  handleFileClose(file);
  return true;
}

void ObjModelParser::handleFileClose(std::ifstream &inFile)
{
  inFile.close();
}

const std::string ObjModelParser::getDiffuseMapFileName()
{
  return m_diffuseMapFileName;
}

const std::string ObjModelParser::getNormalMapFileName()
{
  return m_normalMapFileName;
}

const std::string ObjModelParser::getSpecularMapFileName()
{
  return m_specularMapFileName;
}
