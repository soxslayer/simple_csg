#include <math.h>

#include "vector2.h"

namespace Math
{

Vector2::Vector2 ()
  : x (0), y (0)
{
}

Vector2::Vector2 (float x, float y)
  : x (x), y (y)
{
}

Vector2::Vector2 (const Vector2& v)
  : x (v.x), y (v.y)
{
}

const Vector2& Vector2::operator= (const Vector2& v)
{
  x = v.x;
  y = v.y;

  return *this;
}

const Vector2& Vector2::operator+= (const Vector2& v)
{
  x += v.x;
  y += v.y;

  return *this;
}

const Vector2& Vector2::operator-= (const Vector2& v)
{
  x -= v.x;
  y -= v.y;

  return *this;
}

const Vector2& Vector2::operator*= (float f)
{
  x *= f;
  y *= f;

  return *this;
}

const Vector2& Vector2::operator/= (float f)
{
  x /= f;
  y /= f;

  return *this;
}

const Vector2 Vector2::operator+ (const Vector2& v) const
{
  Vector2 ret = *this;

  ret += v;

  return ret;
}

const Vector2 Vector2::operator- (const Vector2& v) const
{
  Vector2 ret = *this;

  ret -= v;

  return ret;
}

const Vector2 Vector2::operator* (float f) const
{
  Vector2 ret = *this;

  ret *= f;

  return ret;
}

const Vector2 Vector2::operator/ (float f) const
{
  Vector2 ret = *this;

  ret /= f;

  return ret;
}

float Vector2::length () const
{
  return sqrtf (x * x + y * y);
}

Vector2 Vector2::x_axis (1, 0);
Vector2 Vector2::y_axis (0, 1);

} /* namespace Math */



std::ostream& operator<< (std::ostream& os, const Math::Vector2& v)
{
  os << "[ " << v.x << ',' << v.y << " ]";

  return os;
}
