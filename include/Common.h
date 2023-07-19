#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

namespace{


  void log(const char *system, const char *message)
  {
    std::cout << "[" << system << "]" << message << "\n";
  }

  // Loads a file and returns a string
  std::string readFileContent(const std::string &fname, std::string& errorMsg)
  {
    std::string result;
    std::string line;
    std::ifstream myFile(fname.c_str());
    if (myFile.is_open())
    {
      // Get every line of data
      while (getline(myFile, line))
      {
        result += line + '\n';
        // SDL_Log(line); 	// Uncomment this if you want to see
      }
    }
    else
    {
      errorMsg += "file not found. Try an absolute file path to see if the file exists";
    }
    // Close file
    myFile.close();
    return result;
  }
}
