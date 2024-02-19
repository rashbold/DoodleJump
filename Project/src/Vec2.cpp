#include "Vec2.h"

Vec2::Vec2(float xin, float yin)
    : x{xin}, y{yin}
{
}

Vec2::Vec2(const Vec2 &rhs)
    : x{rhs.x}, y{rhs.y}
{
}

bool Vec2::operator==(const Vec2 &rhs) const
{

  return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
  return !(*this == rhs);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
  return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
  return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
  return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
  return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2 &rhs)
{
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
  x *= val;
  y *= val;
}

void Vec2::operator/=(const float val)
{
  if (val != 0)
  {
    x /= val;
    y /= val;
  }
}

float Vec2::dist(const Vec2 &rhs)
{
  Vec2 dist(*this - rhs);
  return sqrt(dist.x * dist.x + dist.y * dist.y);
}

Vec2 Vec2::normalize(float dist)
{
  return Vec2(*this / dist);
}