#include <math.h>

#include "vector4.h"
#include "vector3.h"
#include "matrix4.h"

using namespace std;

namespace Math
{

Vector4::Vector4 ()
  : x (_data[0]), y (_data[1]), z (_data[2]), w (_data[3])
{
  reset ();
}

Vector4::Vector4 (float x, float y, float z, float w)
  : x (_data[0]), y (_data[1]), z (_data[2]), w (_data[3])
{
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

Vector4::Vector4 (const Vector3& v)
  : x (_data[0]), y (_data[1]), z (_data[2]), w (_data[3])
{
  reset ();
}

Vector4::Vector4 (const Vector4& v)
  : x (_data[0]), y (_data[1]), z (_data[2]), w (_data[3])
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

const Vector4& Vector4::operator*= (const Matrix4& m)
{
  float n_x = x * m[0] + y * m[4] + z * m[8] + w * m[12];
  float n_y = x * m[1] + y * m[5] + z * m[9] + w * m[13];
  float n_z = x * m[2] + y * m[6] + z * m[10] + w * m[14];
  float n_w = x * m[3] + y * m[7] + z * m[11] + w * m[15];

  x = n_x;
  y = n_y;
  z = n_z;
  w = n_w;

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
  ret += v;

  return ret;
}

Vector4 Vector4::operator- (const Vector4& v) const
{
  Vector4 ret = *this;
  ret -= v;

  return ret;
}

Vector4 Vector4::operator* (float d) const
{
  Vector4 ret = *this;
  ret *= d;

  return ret;
}

Vector4 Vector4::operator* (const Vector4& v) const
{
  Vector4 ret = *this;
  ret *= v;

  return ret;
}

Vector4 Vector4::operator* (const Matrix4& m) const
{
  Vector4 ret = *this;
  ret *= m;

  return ret;
}

Vector4 Vector4::operator/ (float d) const
{
  Vector4 ret = *this;
  ret /= d;

  return ret;
}

bool Vector4::operator== (const Vector4& v) const
{
  return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator!= (const Vector4& v) const
{
  return !(*this == v);
}

Vector4 Vector4::x_axis = Vector4 (1.0, 0.0, 0.0, 0.0);
Vector4 Vector4::y_axis = Vector4 (0.0, 1.0, 0.0, 0.0);
Vector4 Vector4::z_axis = Vector4 (0.0, 0.0, 1.0, 0.0);
Vector4 Vector4::w_axis = Vector4 (0.0, 0.0, 0.0, 1.0);

ostream& operator<< (ostream& s, const Vector4& v)
{
  s << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";

  return s;
}

} /* namespace Math */
