#include <math.h>

#include "vector3.h"

namespace Math
{

const Vector3 Vector3::x_axis (1, 0, 0);
const Vector3 Vector3::y_axis (0, 1, 0);
const Vector3 Vector3::z_axis (0, 0, 1);

Vector3::Vector3 ()
{
  reset ();
}

Vector3::Vector3 (float x, float y, float z)
  : x (x), y (y), z (z)
{
}

Vector3::Vector3 (const Vector3& v)
{
  x = v.x;
  y = v.y;
  z = v.z;
}

const Vector3& Vector3::operator= (const Vector3& v)
{
  x = v.x;
  y = v.y;
  z = v.z;

  return *this;
}

void Vector3::reset ()
{
  x = 0;
  y = 0;
  z = 0;
}

float Vector3::length () const
{
  return sqrt (x * x + y * y + z * z);
}

float Vector3::dot_product (const Vector3& v) const
{
  return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross_product (const Vector3& v) const
{
  Vector3 ret;

  ret.x = y * v.z - z * v.y;
  ret.y = z * v.x - x * v.z;
  ret.z = x * v.y - y * v.x;

  return ret;
}

const Vector3& Vector3::normalize ()
{
  float l = length ();

  x /= l;
  y /= l;
  z /= l;

  return *this;
}

Vector3 Vector3::normalized () const
{
  Vector3 r = *this;

  r.normalize ();

  return r;
}

float Vector3::angle_between (const Vector3& v) const
{
  float m_l = length ();
  float v_l = v.length ();
  float dp = dot_product (v);

  return acosf (dp / m_l / v_l) * 180.0 / M_PI;
}

const Vector3& Vector3::operator+= (const Vector3& v)
{
  x += v.x;
  y += v.y;
  z += v.z;

  return *this;
}

const Vector3& Vector3::operator-= (const Vector3& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;

  return *this;
}

const Vector3& Vector3::operator*= (float d)
{
  x *= d;
  y *= d;
  z *= d;

  return *this;
}

const Vector3& Vector3::operator/= (float d)
{
  x /= d;
  y /= d;
  z /= d;

  return *this;
}

Vector3 Vector3::operator+ (const Vector3& v) const
{
  Vector3 ret = *this;

  return ret += v;
}

Vector3 Vector3::operator- (const Vector3& v) const
{
  Vector3 ret = *this;

  return ret -= v;
}

Vector3 Vector3::operator* (float d) const
{
  Vector3 ret = *this;

  return ret *= d;
}

Vector3 Vector3::operator/ (float d) const
{
  Vector3 ret = *this;

  return ret /= d;
}

bool Vector3::operator== (const Vector3& v) const
{
  return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!= (const Vector3& v) const
{
  return !(*this == v);
}

Vector3 Vector3::operator- () const
{
  return Vector3 (-x, -y, -z);
}

std::ostream& operator<< (std::ostream& s, const Vector3& v)
{
  s << "[ " << v.x << ',' << v.y << ',' << v.z << " ]";
  return s;
}

} /* namespace Math */
