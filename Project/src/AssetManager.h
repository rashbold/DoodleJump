#pragma once

#include <map>
#include <fstream>
#include <iostream>

#include "Framework.h"

class AssetManager
{
private:
  // is it better if we have a map
  // of smart unique pointers instead?
  std::map<std::string, Sprite *> m_spriteMap;

public:
  AssetManager() = default;
  ~AssetManager();

  void loadFromFile(const std::string &path);
  void addSprite(const std::string &name, const std::string &path);
  Sprite *getSprite(const std::string &name) const;
};
