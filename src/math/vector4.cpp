#include <math.h>

#include "vector4.h"
#include "vector3.h"
#include "matrix4.h"

namespace Math
{

const Vector4 Vector4::x_axis = Vector4 (1, 0, 0, 0);
const Vector4 Vector4::y_axis = Vector4 (0, 1, 0, 0);
const Vector4 Vector4::z_axis = Vector4 (0, 0, 1, 0);
const Vector4 Vector4::w_axis = Vector4 (0, 0, 0, 1);

Vector4::Vector4 ()
{
}

Vector4::Vector4 (float x, float y, float z, float w)
  : Vector<float, 4> ()
{
  set (x, y, z, w);
}

Vector4::Vector4 (const Vector3& v)
  : Vector<float, 4> ()
{
  set (v[0], v[1], v[2], 1);
}

Vector4::Vector4 (const Vector<float, 4>& v)
  : Vector<float, 4> (v)
{
}

Vector4::Vector4 (const Vector4& v)
  : Vector<float, 4> (v)
{
}

const Vector4& Vector4::operator= (const Vector<float, 4>& v)
{
  Vector<float, 4>::operator= (v);
  return *this;
}

const Vector4& Vector4::operator= (const Vector4& v)
{
  Vector<float, 4>::operator= (v);
  return *this;
}

const Vector4& Vector4::operator*= (const Matrix4& m)
{
  float n_x = (*this)[0] * m[0] + (*this)[1] * m[4]
              + (*this)[2] * m[8] + (*this)[3] * m[12];
  float n_y = (*this)[0] * m[1] + (*this)[1] * m[5]
              + (*this)[2] * m[9] + (*this)[3] * m[13];
  float n_z = (*this)[0] * m[2] + (*this)[1] * m[6]
              + (*this)[2] * m[10] + (*this)[3] * m[14];
  float n_w = (*this)[0] * m[3] + (*this)[1] * m[7]
              + (*this)[2] * m[11] + (*this)[3] * m[15];

  set (n_x, n_y, n_z, n_w);

  return *this;
}

Vector4 Vector4::operator* (const Matrix4& m) const
{
  Vector4 ret = *this;
  ret *= m;

  return ret;
}

} /* namespace Math */
