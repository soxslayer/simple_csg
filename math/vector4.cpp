#include <math.h>

#include "vector4.h"
#include "vector3.h"

namespace Math
{

Vector4::Vector4 ()
{
  reset ();
}

Vector4::Vector4 (float x, float y, float z, float w)
  : x (x), y (y), z (z), w (w)
{
}

Vector4::Vector4 (const Vector3& v)
{
  reset ();
}

Vector4::Vector4 (const Vector4& v)
{
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
}

const Vector4& Vector4::operator= (const Vector4& v)
{
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;

  return *this;
}

const Vector4& Vector4::normalize ()
{
  float l = length ();

  x /= l;
  y /= l;
  z /= l;
  w /= l;

  return *this;
}

Vector4 Vector4::normalized ()
{
  Vector4 ret = *this;
  ret.normalize ();

  return ret;
}

float Vector4::length () const
{
  return sqrt (x * x + y * y + z * z + w * w);
}

void Vector4::reset ()
{
  x = 0;
  y = 0;
  z = 0;
  w = 1;
}

const Vector4& Vector4::operator+= (const Vector4& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  w += v.w;

  return *this;
}

const Vector4& Vector4::operator-= (const Vector4& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;
  w -= v.w;

  return *this;
}

const Vector4& Vector4::operator*= (float d)
{
  x *= d;
  y *= d;
  z *= d;
  w *= d;

  return *this;
}

const Vector4& Vector4::operator*= (const Vector4& v)
{
  x *= v.x;
  y *= v.y;
  z *= v.z;
  w *= v.w;

  return *this;
}

const Vector4& Vector4::operator/= (float d)
{
  x /= d;
  y /= d;
  z /= d;
  w /= d;

  return *this;
}

Vector4 Vector4::operator+ (const Vector4& v) const
{
  Vector4 ret = *this;

  return ret += v;
}

Vector4 Vector4::operator- (const Vector4& v) const
{
  Vector4 ret = *this;

  return ret -= v;
}

Vector4 Vector4::operator* (float d) const
{
  Vector4 ret = *this;

  return ret *= d;
}

Vector4 Vector4::operator* (const Vector4& v) const
{
  Vector4 ret = *this;

  return ret *= v;
}

Vector4 Vector4::operator/ (float d) const
{
  Vector4 ret = *this;

  return ret /= d;
}

bool Vector4::operator== (const Vector4& v) const
{
  return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator!= (const Vector4& v) const
{
  return !(*this == v);
}

} /* namespace Math */
