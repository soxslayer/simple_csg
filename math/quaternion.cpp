#include <cmath>

#include "quaternion.h"
#include "vector3.h"

namespace Math
{

Quaternion::Quaternion ()
{
  x = 0;
  y = 0;
  z = 0;
  w = 1;
}

Quaternion::Quaternion (const Vector3& axis, float angle)
{
  float alpha_2 = angle * M_PI / 360;
  float s_alpha_2 = sin (alpha_2);

  w = cos (alpha_2);
  x = axis.x * s_alpha_2;
  y = axis.y * s_alpha_2;
  z = axis.z * s_alpha_2;
}

Quaternion::Quaternion (const Vector4& q)
{
  x = q.x;
  y = q.y;
  z = q.z;
  w = q.w;
}

const Quaternion& Quaternion::operator= (const Quaternion& q)
{
  w = q.w;
  x = q.x;
  y = q.y;
  z = q.z;

  return *this;
}

const Quaternion& Quaternion::conjugate ()
{
  x *= -1;
  y *= -1;
  z *= -1;

  return *this;
}

Quaternion Quaternion::conjugated () const
{
  Quaternion ret = *this;

  return ret.conjugate ();
}

const Quaternion& Quaternion::operator*= (const Quaternion& q)
{
  float n_w = w * q.w - x * q.x - y * q.y - z * q.z;
  float n_x = w * q.x + x * q.w + y * q.z - z * q.y;
  float n_y = w * q.y + y * q.w - x * q.z + z * q.x;
  float n_z = w * q.z + z * q.w + x * q.y - y * q.x;

  w = n_w;
  x = n_x;
  y = n_y;
  z = n_z;

  return *this;
}

Quaternion Quaternion::operator* (const Quaternion& q)
{
  Quaternion ret = *this;

  return ret *= q;
}

std::ostream& operator<< (std::ostream& s, const Quaternion& q)
{
  s << "[ " << q.w << ',' << q.x << ',' << q.y << ',' << q.z << " ]";

  return s;
}

} /* namespace Math */
