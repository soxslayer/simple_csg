#include <cmath>

#include "vector3.h"

namespace Math
{

const Vector3 Vector3::x_axis (1, 0, 0);
const Vector3 Vector3::y_axis (0, 1, 0);
const Vector3 Vector3::z_axis (0, 0, 1);

Vector3::Vector3 ()
  : Vector<float, 3> ()
{
  reset ();
}

Vector3::Vector3 (float x, float y, float z)
  : Vector<float, 3> ()
{
  set (x, y, z);
}

Vector3::Vector3 (const Vector<float, 3>& v)
  : Vector<float, 3> (v)
{
}

Vector3::Vector3 (const Vector3& v)
  : Vector<float, 3> (v)
{
}

const Vector3& Vector3::operator= (const Vector<float, 3>& v)
{
  Vector<float, 3>::operator= (v);
  return *this;
}

const Vector3& Vector3::operator= (const Vector3& v)
{
  Vector<float, 3>::operator= (v);
  return *this;
}

float Vector3::dot_product (const Vector<float, 3>& v) const
{
  return (*this)[0] * v[0] + (*this)[1] * v[1] + (*this)[2] * v[2];
}

Vector3 Vector3::cross_product (const Vector<float, 3>& v) const
{
  Vector3 ret;

  ret[0] = (*this)[1] * v[2] - (*this)[2] * v[1];
  ret[1] = (*this)[2] * v[0] - (*this)[0] * v[2];
  ret[2] = (*this)[0] * v[1] - (*this)[1] * v[0];

  return ret;
}

float Vector3::angle_between (const Vector<float, 3>& v) const
{
  float m_l = length ();
  float v_l = v.length ();
  float dp = dot_product (v);

  return acosf (dp / m_l / v_l) * 180.0 / M_PI;
}

} /* namespace Math */
