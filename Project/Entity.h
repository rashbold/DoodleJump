#pragma once

#include <string>
#include "Components.h"

class Entity
{
  friend class EntityManager;

  bool m_active = true;
  size_t m_id = 0;
  std::string m_tag;

  Entity() = delete;
  // what's the advantage of making this const?
  Entity(const size_t id, const std::string &tag);

public:
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CCollision> cCollision;
  std::shared_ptr<CSprite> cSprite;
  std::shared_ptr<CBoundingBox> cBoundingBox;
  std::shared_ptr<CInput> cInput;
  std::shared_ptr<CScore> cScore;
  std::shared_ptr<CLifespan> cLifespan;
  std::shared_ptr<CPlatform> cPlatform;
  std::shared_ptr<CSpecialAbility> cSpecialAbility;

  bool isActive() const;
  const std::string &tag() const;
  const size_t id() const;
  void destroy();
};
