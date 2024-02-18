#include "Physics.h"

#include <iostream>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
  auto transformA = a->cTransform;
  auto transformB = b->cTransform;
  auto colliderA = a->cBoundingBox;
  auto colliderB = b->cBoundingBox;

  auto aBB = a->cBoundingBox->halfSize;
  auto bBB = b->cBoundingBox->halfSize;

  auto aPos = a->cTransform->position + aBB;
  auto bPos = b->cTransform->position + bBB;

  auto delta = Vec2(abs(aPos.x - bPos.x), abs(aPos.y - bPos.y));
  auto ox = aBB.x + bBB.x - delta.x;
  auto oy = aBB.y + bBB.y - delta.y;

  return Vec2(ox, oy);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
  auto aBB = a->cBoundingBox->halfSize;
  auto bBB = b->cBoundingBox->halfSize;

  auto aPos = a->cTransform->prevPos + aBB;
  auto bPos = b->cTransform->prevPos + bBB;

  auto delta = Vec2(abs(aPos.x - bPos.x), abs(aPos.y - bPos.y));
  auto ox = aBB.x + bBB.x - delta.x;
  auto oy = aBB.y + bBB.y - delta.y;

  return Vec2(ox, oy);
}
