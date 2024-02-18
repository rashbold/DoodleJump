#pragma once

#include <map>
#include <fstream>
#include <iostream>

#include "Framework.h"

class AssetManager
{
private:
  // I asssume it's better if we have a map of unique pointers instead
  std::map<std::string, Sprite *> m_spriteMap;

public:
  void loadFromFile(const std::string &path);
  void addSprite(const std::string &name, const std::string &path);
  Sprite *getSprite(const std::string &name) const;
};
