#pragma once
#include "math.h"

class Vec2
{
public:
  float x;
  float y;

  // NOTE: since we use an implicit default constructor
  // and provide no default initialization value,
  // x & y will initialized to 0 before constructed.
  // read: https://www.learncpp.com/cpp-tutorial/default-constructors-and-default-arguments/
  Vec2() = default;
  Vec2(float xin, float yin);
  Vec2(const Vec2 &rhs);

  bool operator==(const Vec2 &rhs) const;
  bool operator!=(const Vec2 &rhs) const;

  Vec2 operator+(const Vec2 &rhs) const;
  Vec2 operator-(const Vec2 &rhs) const;
  Vec2 operator/(const float val) const;
  Vec2 operator*(const float val) const;

  void operator+=(const Vec2 &rhs);
  void operator-=(const Vec2 &rhs);
  void operator*=(const float val);
  void operator/=(const float val);

  float dist(const Vec2 &rhs);
  Vec2 normalize(float dist);
};