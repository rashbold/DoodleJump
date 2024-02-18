

#include "Game.h"

int main(int argc, char *argv[])
{
  int wWidth = 800;
  int wHeight = 600;

  if (argc >= 4 && !strcmp(argv[1], "-window"))
  {
    std::cout << "Invalid command line arguments" << std::endl;
    wWidth = std::stoi(argv[2]);
    wHeight = std::stoi(argv[3]);
  }

  auto game = new Game("assets.txt", wWidth, wHeight);
  run(game);
}
