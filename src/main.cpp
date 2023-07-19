#include <iostream>
#include <string>

#include "SDLGraphicsProgram.h"

int main(int argc, char *argv[])
{
  SDLGraphicsProgram::instance().setScreenSize(640, 480);
  SDLGraphicsProgram::instance().loadModel();
  SDLGraphicsProgram::instance().loop();

  return 0;
}