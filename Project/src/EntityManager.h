#pragma once

#include <map>
#include "Entity.h"

using EnitityVec = std::vector<std::shared_ptr<Entity>>;
using EnitityMap = std::map<std::string, EnitityVec>;

class EntityManager
{
  EnitityVec m_entities;
  EnitityVec m_entitiesToAdd;
  EnitityMap m_entityMap;
  size_t m_totalEntities = 0;

  void removeDeadEntities(EnitityVec &vec);

public:
  EntityManager();

  void update();

  std::shared_ptr<Entity> addEntity(const std::string &tag);

  EnitityVec getEntities();
  EnitityVec getEntities(const std::string &tag);
  // need to iterate via map and get different enities tags
  const std::map<std::string, EnitityVec> &getEntityMap();
};