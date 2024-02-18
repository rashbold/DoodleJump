#include "Game.h"

int main(int argc, char *argv[])
{
  int wWidth = 800;
  int wHeight = 600;

  if (argc >= 4 && !strcmp(argv[1], "-window"))
  {
    try
    {
      wWidth = std::stoi(argv[2]);
      wHeight = std::stoi(argv[3]);
    }
    catch (const std::invalid_argument &e)
    {
      std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
  }

  auto game = new Game("assets.txt", wWidth, wHeight);
  run(game);
}
