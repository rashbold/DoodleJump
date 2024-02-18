#pragma once
#include <iostream>
#include <fstream>
#include <random>

#include "Action.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "Framework.h"

using ActionMap = std::map<int, std::string>;

constexpr int FPS = 60;
constexpr int MPF = 1000 / FPS;

class Game : public Framework
{
  EntityManager m_entities;
  AssetManager m_assets;
  ActionMap m_actionMap;

  std::string m_congigPath;
  Vec2 m_windowSize;

  unsigned m_score = 0;
  unsigned int m_totalPlatforms = 0;
  unsigned int m_currentPlatform = 1;
  unsigned int m_lastJumpedPlatform = 1;
  Vec2 m_lastPlatformPos = Vec2(0.0f, 0.0f);
  Vec2 m_mousePos;
  std::random_device rand_dev;
  std::mt19937 rand_generator;
  int m_cameraYOffset = 0;

  float m_jumpTime = 0.45f;
  float m_maxJumpTime = 3 * m_jumpTime;
  float m_minJumpTime = 0.5f * m_jumpTime;

  std::shared_ptr<Entity> m_player;

public:
  unsigned int millisecondsAtPreviousFrame = 0;

  Game(const std::string &path, int wWidth, int wHeight);

  virtual void PreInit(int &width, int &height, bool &fullscreen) override;
  virtual bool Init() override;
  virtual void Close() override;
  virtual bool Tick() override;
  virtual void onMouseMove(int x, int y, int xrelative, int yrelative) override;
  virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) override;
  virtual void onKeyPressed(FRKey k) override;
  virtual void onKeyReleased(FRKey k) override;
  virtual const char *GetTitle() override;

  void spawnPlayer();
  void spawnEnemy(std::shared_ptr<Entity> e);
  void spawnSpecialAbility(std::shared_ptr<Entity> e);
  void spawnPlatform(Vec2 &pos);
  void spawnBullet(Vec2 &target);

  void setupScene();

  void sMovement();
  void sRender();
  void sCollision();
  void sScoreRender();
  void sPlatformSpawner();
  void sUserInput();
  void sCleaner();
};