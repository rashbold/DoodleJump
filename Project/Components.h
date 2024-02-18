#pragma once

#include "Framework.h"

#include "Vec2.h"

class CTransform
{
public:
  Vec2 position = {0.0, 0.0};
  Vec2 prevPos = {0.0, 0.0};
  Vec2 velocity = {0.0, 0.0};
  // dis: what's the diff between {} and below initializers
  float angle = 0.0;

  // dis: what does exactly default constructor mean?
  CTransform() = default;
  CTransform(const Vec2 &p, const Vec2 &v, float a)
      : position{p}, prevPos{p}, velocity{v}, angle{a}
  {
  }
};

class CCollision
{
public:
  float radius = {0.0};

  CCollision() = default;
  CCollision(float r) : radius(r)
  {
  }
};

class CScore
{
public:
  int score = {0};

  CScore() = default;
  CScore(int s) : score(s)
  {
  }
};

class CLifespan
{
public:
  int total = {0};
  int remaining = {0};

  CLifespan() = default;
  CLifespan(int total) : total(total), remaining(total)
  {
  }
};

class CInput
{
public:
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

struct CGravity
{
  float gravity = 0.0f;

  CGravity() = default;
  CGravity(float g) : gravity(g)
  {
  }
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
