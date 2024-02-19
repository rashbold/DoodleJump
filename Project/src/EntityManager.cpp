#include "EntityManager.h"

#include <iostream>

void EntityManager::removeDeadEntities(EnitityVec &vec)
{

  auto condition = [](std::shared_ptr<Entity> e)
  { return !e->isActive(); };

  // Use std::remove_if to move the elements that should be removed to the end
  // of the vector, and get the iterator to the new end of the range.
  auto newEnd = std::remove_if(vec.begin(), vec.end(), condition);

  // Erase the "removed" elements from the vector.
  vec.erase(newEnd, vec.end());
}

EntityManager::EntityManager()
{
  m_entities.reserve(10);
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{

  std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
  m_entitiesToAdd.push_back(entity);
  return entity;
}

EnitityVec EntityManager::getEntities()
{
  return m_entities;
}

EnitityVec EntityManager::getEntities(const std::string &tag)
{
  return m_entityMap[tag];
}

void EntityManager::update()
{
  for (auto &e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->m_tag].push_back(e);
  }

  for (auto &[tag, enityVec] : m_entityMap)
  {
    removeDeadEntities(enityVec);
  }
  removeDeadEntities(m_entities);

  m_entitiesToAdd.clear();
}
