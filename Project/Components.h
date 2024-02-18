#pragma once

#include "Framework.h"
#include "Vec2.h"

struct CTransform
{
  Vec2 position = {0.0, 0.0};
  Vec2 prevPos = {0.0, 0.0};
  Vec2 velocity = {0.0, 0.0};
  float angle = 0.0;

  CTransform() = default;
  CTransform(const Vec2 &p, const Vec2 &v, float a)
      : position{p}, prevPos{p}, velocity{v}, angle{a}
  {
  }
};

struct CCollision
{
  float radius = {0.0};

  CCollision() = default;
  CCollision(float r) : radius(r)
  {
  }
};

struct CScore
{
  int score = {0};

  CScore() = default;
  CScore(int s) : score(s)
  {
  }
};

struct CLifespan
{
  unsigned int totalLifespan = 2000;
  unsigned int instantiationTime;

  CLifespan() = default;
  CLifespan(unsigned int t, unsigned int it) : totalLifespan(t), instantiationTime(it)
  {
  }
};

struct CInput
{
  bool left = false;
  bool right = false;
  bool up = false;
  bool down = false;
  bool shoot = false;

  CInput() = default;
};

struct CSprite
{
  Sprite *s;

  CSprite() = default;
  CSprite(Sprite *s) : s(s)
  {
  }
};

struct CBoundingBox
{
public:
  Vec2 size;
  Vec2 halfSize;
  CBoundingBox() = default;
  CBoundingBox(const Vec2 &s) : size{s}, halfSize{s / 2} {}
};

struct CPlatform
{
  bool scored = false;
  unsigned int score = 0;
  CPlatform() = default;
  CPlatform(unsigned int s) : score(s)
  {
  }
};

struct CSpecialAbility
{
  std::string type = "rocket";
  unsigned int totalLifespan = 10000;
  unsigned int instantiationTime;

  CSpecialAbility() = default;
  CSpecialAbility(std::string t, unsigned int tl, unsigned int it) : type(t), totalLifespan(tl), instantiationTime(it)
  {
  }
};
