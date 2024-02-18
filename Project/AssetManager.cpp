#include "AssetManager.h"
#include <cassert>

void AssetManager::loadFromFile(const std::string &path)
{
  std::ifstream file(path);

  while (file.good())
  {
    std::string name, path;
    file >> name >> path;
    addSprite(name, path);
  }
}

void AssetManager::addSprite(const std::string &name, const std::string &path)
{
  auto sprite = FRAMEWORK_API::createSprite(path.c_str());

  if (sprite)
  {
    m_spriteMap[name] = sprite;
  }
}

Sprite *AssetManager::getSprite(const std::string &name) const
{
  assert(m_spriteMap.find(name) != m_spriteMap.end());
  return m_spriteMap.at(name);
}
