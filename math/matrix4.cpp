#include "matrix4.h"
#include "quaternion.h"
#include "vector3.h"

namespace Math
{

const Matrix4 Matrix4::identity;

Matrix4::Matrix4 ()
{
  reset ();
}

Matrix4::Matrix4 (const Matrix4& m)
{
  set_data (m.data ());
}

Matrix4::Matrix4 (const Quaternion& q)
{
  float aa = q.w * q.w;
  float bb = q.x * q.x;
  float cc = q.y * q.y;
  float dd = q.z * q.z;

  float bc = q.x * q.y;
  float ad = q.w * q.z;
  float bd = q.x * q.z;
  float ac = q.w * q.y;
  float cd = q.y * q.z;
  float ab = q.w * q.x;

  _d[0] = aa + bb - cc - dd;
  _d[1] = 2 * (bc - ad);
  _d[2] = 2 * (bd + ac);
  _d[3] = 0;
  _d[4] = 2 * (bc + ad);
  _d[5] = aa - bb + cc - dd;
  _d[6] = 2 * (cd - ab);
  _d[7] = 0;
  _d[8] = 2 * (bd - ac);
  _d[9] = 2 * (cd + ab);
  _d[10] = aa - bb - cc + dd;
  _d[11] = 0;
  _d[12] = 0;
  _d[13] = 0;
  _d[14] = 0;
  _d[15] = 1;
}

const Matrix4& Matrix4::operator= (const Matrix4& m)
{
  if (&m != this)
    set_data (m.data ());

  return *this;
}

const Matrix4& Matrix4::set_data (const float* d)
{
  for (int i = 0; i < 16; ++i)
    _d[i] = d[i];

  return *this;
}

const Matrix4& Matrix4::operator*= (const Matrix4& m)
{
  float tmp[16];

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      tmp[i * 4 + j] = _d[i * 4] * m[j] + _d[i * 4 + 1] * m[4 + j]
                       + _d[i * 4 + 2] * m[8 + j] + _d[i * 4 + 3] * m[12 + j];
    }
  }

  set_data (tmp);

  return *this;
}

Matrix4 Matrix4::operator* (const Matrix4& m) const
{
  Matrix4 ret = *this;
  return ret *= m;
}

const Matrix4& Matrix4::translate (const Vector3& v)
{
  return translate (v.x, v.y, v.z);
}

const Matrix4& Matrix4::translate (float x, float y, float z)
{
  Matrix4 tm;

  tm[12] = x;
  tm[13] = y;
  tm[14] = z;

  *this *= tm;

  return *this;
}

Matrix4 Matrix4::translated (const Vector3& v) const
{
  Matrix4 ret = *this;

  return ret.translate (v);
}

Matrix4 Matrix4::translated (float x, float y, float z) const
{
  Matrix4 ret = *this;

  return ret.translate (x, y, z);
}

const Matrix4& Matrix4::rotate (const Vector3& axis, float angle)
{
  return rotate (Quaternion (axis, angle));
}

const Matrix4& Matrix4::rotate (float x, float y, float z, float angle)
{
  return rotate (Vector3 (x, y, z), angle);
}

const Matrix4& Matrix4::rotate (const Quaternion& q)
{
  Matrix4 rm = q;

  return *this *= rm;
}

const Matrix4& Matrix4::rotate (float phi, float theta, float rho)
{
  Quaternion rp (Vector3::x_axis, phi);
  Quaternion rt (Vector3::y_axis, theta);
  Quaternion rr (Vector3::z_axis, rho);

  return rotate (rp * rt * rr);
}

Matrix4 Matrix4::rotated (const Vector3& axis, float angle) const
{
  Matrix4 ret = *this;

  return ret.rotate (axis, angle);
}

Matrix4 Matrix4::rotated (float x, float y, float z, float angle) const
{
  Matrix4 ret = *this;

  return ret.rotate (x, y, z, angle);
}

Matrix4 Matrix4::rotated (const Quaternion& q) const
{
  Matrix4 ret = *this;

  return ret.rotate (q);
}

Matrix4 Matrix4::rotated (float phi, float theta, float rho) const
{
  Matrix4 ret = *this;

  return ret.rotate (phi, theta, rho);
}

const Matrix4& Matrix4::scale (float x, float y, float z)
{
  Matrix4 rm;

  rm[0] = x;
  rm[5] = y;
  rm[10] = z;

  return *this *= rm;
}

Matrix4 Matrix4::scaled (float x, float y, float z) const
{
  Matrix4 ret = *this;

  return ret.scale (x, y, z);
}

const Matrix4& Matrix4::transpose ()
{
  Matrix4 t;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j)
      t[j * 4 + i] = _d[i * 4 + j];
  }

  *this = t;

  return *this;
}

Matrix4 Matrix4::transposed () const
{
  Matrix4 r = *this;

  r.transpose ();

  return r;
}

void Matrix4::reset ()
{
  for (int i = 0; i < 16; ++i)
    _d[i] = i % 5 == 0 ? 1 : 0;
}

void Matrix4::look_at (const Vector3& eye_location,
                       const Vector3& eye_direction,
                       const Vector3& up_vector)
{
  reset ();

  translate (-eye_location);

  Vector3 axis = eye_direction.cross_product (Vector3::z_axis);
  float angle = 180 - eye_direction.angle_between (Vector3::z_axis);

  if (angle != 0)
    rotate (axis, angle);

  angle = up_vector.angle_between (Vector3::y_axis);

  rotate (Vector3::z_axis, -angle);
}

void Matrix4::ortho_projection (float left, float right, float bottom,
                                float top, float near, float far)
{
  reset ();

  translate (-(left + right) / 2, -(top + bottom) / 2, -(far + near) / 2);
  scale (2 / (right - left), 2 / (top - bottom), 2 / (far - near));
}

std::ostream& operator<< (std::ostream& s, const Matrix4& m)
{
  s << "[[ " << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << " ]"
    << std::endl;
  s << " [ " << m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << " ]"
    << std::endl;
  s << " [ " << m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << " ]"
    << std::endl;
  s << " [ " << m[12] << ", " << m[13] << ", " << m[14] << ", "
    << m[15] << " ]]";

  return s;
}

} /* namespace Math */
