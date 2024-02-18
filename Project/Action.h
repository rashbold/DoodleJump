#pragma once

#include <string>

class Action
{
  std::string m_name = "none";
  // better to do this as an enum
  std::string m_type = "none";

public:
  Action();
  Action(const std::string &name, const std::string &type);

  const std::string &getName() const;
  const std::string &getType() const;
  std::string toString() const;

  // std::map<int, std::string>
  // int = sfml key code for keyboard key
  // string = action name to be performed
};