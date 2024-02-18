#include "Game.h"
#include <chrono>
#include <thread>
#include <random>
#include "Physics.h"

constexpr int FPS = 60;
constexpr int MPF = 1000 / FPS;

Game::Game(const std::string &config, int wWidth, int wHeight)
{
  m_congigPath = config;
  m_windowSize = Vec2(wWidth, wHeight);
  rand_generator = std::mt19937(std::random_device{}());
}

bool Game::Tick()
{
  unsigned int timeToWait = MPF - (getTickCount() - millisecondsAtPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MPF)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(timeToWait));
  }
  millisecondsAtPreviousFrame = getTickCount();

  m_entities.update();

  sUserInput();
  sMovement();
  sCollision();
  // drawTestBackground();
  sCleaner();
  sPlatformSpawner();
  sScoreRender();
  sRender();

  return false;
}

void Game::sMovement()
{
  auto playerGravity = m_player->cGravity->gravity;
  m_player->cTransform->velocity.y += playerGravity;

  for (auto e : m_entities.getEntities())
  {
    e->cTransform->prevPos = e->cTransform->position;
    e->cTransform->position += e->cTransform->velocity;
  }

  // Check if the player is 200 pixels below the top of the game window
  if (m_player->cTransform->position.y < m_cameraYOffset + 200)
  {
    m_cameraYOffset = m_player->cTransform->position.y - 200;
  }
}

void Game::sRender()
{
  for (auto e : m_entities.getEntities())
  {
    if (e->cBoundingBox && e->cSprite)
    {
      auto sprite = e->cSprite->s;
      auto pos = e->cTransform->position;
      drawSprite(sprite, pos.x, pos.y - m_cameraYOffset);
    }
    else
    {
    }
  }
}

void Game::sLifespan()
{
  for (auto e : m_entities.getEntities())
  {
    auto lf = e->cLifespan;
    if (!lf)
      continue;

    if (lf->remaining > 0)
    {
      lf->remaining -= 1.0f;
      auto uO = (float(lf->remaining) / lf->total) * 255.0f;

      printf("remaining: %d, total: %d, uO: %f\n", lf->remaining, lf->total, uO);
    }
    else
    {
      e->destroy();
    }
  }
}

void Game::sCollision()
{
  // coll of player with platforms
  for (auto &platform : m_entities.getEntities("platform"))
  {
    if (m_player->cSpecialAbility)
      break;

    auto overlap = Physics::GetOverlap(m_player, platform);
    auto previousOverlap = Physics::GetPreviousOverlap(m_player, platform);

    if (overlap.x < 0 || overlap.y < 0 || previousOverlap.x < 0)
      continue;

    auto playerPos = m_player->cTransform->position;
    auto playerPreviousPos = m_player->cTransform->prevPos;
    auto platformPos = platform->cTransform->position;

    auto playerBBOX = m_player->cBoundingBox->size;
    auto playerPreviousBottom = playerPreviousPos.y + playerBBOX.y;
    auto platformTop = platformPos.y;
    bool movingDown = playerPos.y > playerPreviousPos.y;

    // moving down and was above
    if (movingDown && playerPreviousBottom <= platformTop)
    {
      m_currentPlatform = platform->cPlatform->score;
      std::cout << "Current platform: " << m_currentPlatform << std::endl;

      m_player->cTransform->position.y -= overlap.y;
      m_player->cTransform->velocity.y = -15.0f;
    }
  }

  // coll of player with enemies
  for (auto enemy : m_entities.getEntities("enemy"))
  {
    if (m_player->cSpecialAbility)
      break;

    auto overlap = Physics::GetOverlap(m_player, enemy);
    if (overlap.x < 0 || overlap.y < 0)
      continue;

    auto previousOverlap = Physics::GetPreviousOverlap(m_player, enemy);
    if (previousOverlap.x < 0)
      continue;

    auto playerPos = m_player->cTransform->position;
    auto playerPreviousPos = m_player->cTransform->prevPos;
    auto platformPos = enemy->cTransform->position;

    if (playerPreviousPos.y < playerPos.y && playerPreviousPos.y + m_player->cBoundingBox->size.y <= platformPos.y)
      enemy->destroy();
    else
      setupScene();
  }

  // coll of enemy with bullets
  for (auto enemy : m_entities.getEntities("enemy"))
  {
    for (auto bullet : m_entities.getEntities("bullet"))
    {
      auto overlap = Physics::GetOverlap(enemy, bullet);
      if (overlap.x < 0 || overlap.y < 0)
        continue;

      enemy->destroy();
      bullet->destroy();
    }
  }

  // coll of player with walls
  Vec2 pos = m_player->cTransform->position;
  Vec2 size = m_player->cBoundingBox->size;

  if (pos.x + size.x < 0)
    m_player->cTransform->position.x = m_windowSize.x - size.x;
  if (pos.x > m_windowSize.x)
    m_player->cTransform->position.x = 0;
  if (pos.y + size.y > m_windowSize.y)
    setupScene();

  // coll of bullets with walls
  for (auto bullet : m_entities.getEntities("bullet"))
  {
    Vec2 &pos = bullet->cTransform->position;
    Vec2 size = bullet->cBoundingBox->size;
    if (pos.x + size.x < 0)
      pos.x = m_windowSize.x - size.x;
    if (pos.x > m_windowSize.x)
      pos.x = 0;
  }

  // coll of player with abilities
  for (auto ability : m_entities.getEntities("Box"))
  {
    if (m_player->cSpecialAbility)
      break;

    auto overlap = Physics::GetOverlap(m_player, ability);
    if (overlap.x < 0 || overlap.y < 0)
      continue;

    ability->destroy();
    m_player->cSpecialAbility = std::make_shared<CSpecialAbility>("Rocket", 2000, getTickCount());
    auto abilitySprite = m_assets.getSprite("Puca");
    m_player->cSprite = std::make_shared<CSprite>(abilitySprite);
    m_player->cGravity->gravity = 0;
    m_player->cTransform->velocity.y = -20.0f;
  }
}

void Game::spawnBullet(Vec2 &target)
{
  // TODO: this has to be in Vec2 class,
  // I don't have to make it manually
  Vec2 dest = target - (m_player->cTransform->position - Vec2(0, m_cameraYOffset));
  float dist = sqrt(dest.x * dest.x + dest.y * dest.y);
  Vec2 d = dest.normalize(dist);

  d *= 10;

  auto entity = m_entities.addEntity("bullet");
  entity->cSprite = std::make_shared<CSprite>(m_assets.getSprite("Bullet"));

  int w, h;
  getSpriteSize(m_assets.getSprite("Bullet"), w, h);

  entity->cTransform = std::make_shared<CTransform>(m_player->cTransform->position, d, 0.0f);
  entity->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(w, h));
}

void Game::spawnPlayer()
{
  auto player = m_entities.addEntity("player");
  auto sprite = m_assets.getSprite("Right");
  player->cSprite = std::make_shared<CSprite>(sprite);

  // TODO: move to asset manager and set automatically when load sprite
  int w = 0;
  int h = 0;
  getSpriteSize(sprite, w, h);

  // position at the bottom of the screen
  float yPos = m_windowSize.y - h - 20.0f;

  player->cTransform = std::make_shared<CTransform>(Vec2(0.0f, yPos), Vec2(0.0f, 0.0f), 0.0f);
  player->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(w, h));
  player->cLifespan = std::make_shared<CLifespan>(100);
  player->cGravity = std::make_shared<CGravity>(0.6f);
  player->cInput = std::make_shared<CInput>();
  player->cInput = std::make_shared<CInput>();

  m_player = player;
  m_lastPlatformPos = player->cTransform->position;

  // make non constant ref option of the function
  Vec2 initialPosition = Vec2(0, m_windowSize.y - 16);
  spawnPlatform(initialPosition);
}

void Game::sScoreRender()
{

  auto score = m_currentPlatform;
  std::vector<int> digits;
  digits.push_back(score % 10);
  score /= 10;
  while (score > 0)
  {
    digits.push_back(score % 10);
    score /= 10;
  }

  std::reverse(digits.begin(), digits.end());

  int lastDigitW = 0;

  for (auto d : digits)
  {
    auto sprite = m_assets.getSprite(std::to_string(d));
    int w, h;
    getSpriteSize(sprite, w, h);

    drawSprite(sprite, 10 + lastDigitW, 10);
    lastDigitW += w;
  }
}

void Game::spawnEnemy(std::shared_ptr<Entity> e)
{
  auto enemy = m_entities.addEntity("enemy");
  auto sprite = m_assets.getSprite("Enemy");
  enemy->cSprite = std::make_shared<CSprite>(sprite);

  auto pTransform = e->cTransform;
  auto pSprite = e->cSprite;

  int pW = 0;
  int pH = 0;
  getSpriteSize(pSprite->s, pW, pH);

  int w = 0;
  int h = 0;
  getSpriteSize(sprite, w, h);

  // position at top center of the platform
  float yPos = pTransform->position.y - h;
  float xPos = pTransform->position.x + pW / 2.0f - w / 2.0f;

  enemy->cTransform = std::make_shared<CTransform>(Vec2(xPos, yPos), Vec2(0.0f, 0.0f), 0.0f);
  enemy->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(w, h));
}

void Game::spawnSpecialAbility(std::shared_ptr<Entity> e)
{
  // TODO: choose random special ability
  // auto newPlayerSprite = m_assets.getSprite("Puca");
  auto ability = m_entities.addEntity("Box");
  auto sprite = m_assets.getSprite("Box");

  int w, h;
  getSpriteSize(sprite, w, h);

  Vec2 abilityPos = Vec2((e->cTransform->position.x + e->cBoundingBox->halfSize.x - w / 2.0f), e->cTransform->position.y - h);

  ability->cSprite = std::make_shared<CSprite>(sprite);
  ability->cTransform = std::make_shared<CTransform>(abilityPos, Vec2(0.0f, 0.0f), 0.0f);
  ability->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(w, h));
}

void Game::sPlatformSpawner()
{
  int totalPlatforms = m_entities.getEntities("platform").size();
  // std::cout << "total platforms: " << totalPlatforms << std::endl;

  // TODO: # have to also count platform size
  if (m_lastPlatformPos.y < m_cameraYOffset || m_lastPlatformPos.y > m_cameraYOffset + m_windowSize.y)
  {
    return;
  }

  std::uniform_int_distribution<int> pwDist(50, 150);
  std::uniform_int_distribution<int> nwDist(-150, -50);
  std::uniform_int_distribution<int> hDist(-120, -30);
  std::uniform_int_distribution<int> chooser(0, 1);
  std::uniform_int_distribution<int> isEnemyTime(1, 25);

  int ranX = chooser(rand_generator) ? pwDist(rand_generator) : nwDist(rand_generator);
  int rany = hDist(rand_generator);

  auto newPlatformPos = Vec2(m_lastPlatformPos + Vec2(ranX, rany));

  spawnPlatform(newPlatformPos);
}

void Game::spawnPlatform(Vec2 &pos)
{

  auto platform = m_entities.addEntity("platform");
  auto sprite = m_assets.getSprite("Platform");

  int w = 0;
  int h = 0;
  getSpriteSize(sprite, w, h);

  platform->cSprite = std::make_shared<CSprite>(sprite);

  std::uniform_int_distribution<int> isEnemyTime(1, 25);
  std::uniform_int_distribution<int> isSpecialAbilityTime(1, 30);

  if (pos.x + w < 0)
    pos.x = m_windowSize.x - w;
  if (pos.x > m_windowSize.x - w)
    pos.x = 0;

  m_totalPlatforms++;

  platform->cTransform = std::make_shared<CTransform>(pos, Vec2(0.0f, 0.0f), 0.0f);
  platform->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(w, h));
  platform->cPlatform = std::make_shared<CPlatform>(m_totalPlatforms);

  m_lastPlatformPos = platform->cTransform->position;

  if (isEnemyTime(rand_generator) == 1)
  {
    spawnEnemy(platform);
  }
  else if (isSpecialAbilityTime(rand_generator) == 1)
  {
    spawnSpecialAbility(platform);
  }
}

void Game::sUserInput()
{
  auto playerInput = m_player->cInput;
  auto &playerVelocity = m_player->cTransform->velocity;

  playerVelocity.x = 0.0f;

  if (playerInput->right)
  {
    if (!m_player->cSpecialAbility)
    {
      playerVelocity.x += 3.5f;
      m_player->cSprite->s = m_assets.getSprite("Right");
    }
    else
      playerVelocity.x += 5.5f;
  }

  if (playerInput->left)
  {
    if (!m_player->cSpecialAbility)
    {
      playerVelocity.x -= 3.5f;
      m_player->cSprite->s = m_assets.getSprite("Left");
    }
    else
      playerVelocity.x -= 5.5f;
  }
}

void Game::sCleaner()
{

  for (auto e : m_entities.getEntities("platform"))
  {
    if (e->cTransform->position.y > m_cameraYOffset + m_windowSize.y)
      e->destroy();
  }

  // TODO: clean up boolets
  for (auto e : m_entities.getEntities("bullet"))
  {
    if (e->cTransform->position.y > m_cameraYOffset + m_windowSize.y || e->cTransform->position.y < m_cameraYOffset)
      e->destroy();
  }

  // player fell off
  if (m_player->cTransform->position.y > m_cameraYOffset + m_windowSize.y)
    setupScene();

  // clean special ability
  if (m_player->cSpecialAbility)
  {
    auto ability = m_player->cSpecialAbility;
    if (getTickCount() - ability->instantiationTime > ability->totalLifespan)
    {
      m_player->cSpecialAbility = nullptr;
      auto playerSprite = m_assets.getSprite("Right");
      m_player->cGravity->gravity = 0.6f;
      m_player->cSprite->s = playerSprite;
    }
  }
}

void Game::setupScene()
{
  for (auto &e : m_entities.getEntities())
  {
    e->destroy();
  }
  m_totalPlatforms = 0;
  m_currentPlatform = 1;
  spawnPlayer();
  m_cameraYOffset = 0;
}

void Game::PreInit(int &width, int &height, bool &fullscreen)
{
  width = m_windowSize.x;
  height = m_windowSize.y;
  fullscreen = false;
}

bool Game::Init()
{
  m_assets.loadFromFile(m_congigPath);
  srand((unsigned)std::time(NULL));
  m_cameraYOffset = 0;
  spawnPlayer();
  return true;
}

void Game::Close()
{
  // deallocate sprites?
}

void Game::onMouseMove(int x, int y, int xrelative, int yrelative)
{
  m_mousePos.x = x;
  m_mousePos.y = y;
}

void Game::onMouseButtonClick(FRMouseButton button, bool isReleased)
{
  if (button == FRMouseButton::LEFT && isReleased && !m_player->cSpecialAbility)
  {
    Vec2 target = Vec2(m_mousePos.x, m_mousePos.y);
    spawnBullet(target);
  }
}

void Game::onKeyPressed(FRKey k)
{
  switch (k)
  {
  case FRKey::RIGHT:
    m_player->cInput->right = true;
    break;
  case FRKey::LEFT:
    m_player->cInput->left = true;
    break;
  }
}

void Game::onKeyReleased(FRKey k)
{
  switch (k)
  {
  case FRKey::RIGHT:
    m_player->cInput->right = false;
    break;
  case FRKey::LEFT:
    m_player->cInput->left = false;
    break;
  }
}

const char *Game::GetTitle()
{
  return "Dursley Jump";
}
