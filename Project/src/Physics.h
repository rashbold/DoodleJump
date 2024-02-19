#pragma once

#include "Vec2.h"
#include "Entity.h"

constexpr float JUMP_HEIGHT = 4.5f;

class Physics
{
private:
  /* data */
public:
  Physics(/* args */);
  ~Physics();

  static Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
  static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

  static float GetJumpVelocity(float timeToPeak);
  static float GetJumpGravity(float timeToPeak);
};
