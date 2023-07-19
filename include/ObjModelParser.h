#pragma once

#include "ModelParser.h"

class ObjModelParser : public ModelParser
{
public:
  bool parseModel(std::string modelFileName) override;
  const std::string getDiffuseMapFileName() override;
  const std::string getNormalMapFileName() override;
  const std::string getSpecularMapFileName() override;

private:
  void handleFileClose(std::ifstream &inFile);

  std::string m_diffuseMapFileName;
  std::string m_normalMapFileName;
  std::string m_specularMapFileName;
};