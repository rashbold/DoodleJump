#include "GameEngine.h"

void GameEngine::PreInit(int &width, int &height, bool &fullscreen)
{
  width = 320;
  height = 200;
  fullscreen = false;
}

bool GameEngine::Init()
{
  return true;
}

void GameEngine::Close()
{
}

bool GameEngine::Tick()
{

  drawTestBackground();
  return false;
}

void GameEngine::onMouseMove(int x, int y, int xrelative, int yrelative)
{
}

void GameEngine::onMouseButtonClick(FRMouseButton button, bool isReleased)
{
}

void GameEngine::onKeyPressed(FRKey k)
{
}

void GameEngine::onKeyReleased(FRKey k)
{
}

const char *GameEngine::GetTitle()
{
  return "Arcanoid";
}
