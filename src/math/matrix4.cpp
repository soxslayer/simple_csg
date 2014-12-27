#include "matrix4.h"
#include "quaternion.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix2.h"

namespace Math
{

Matrix4::Matrix4 ()
  : Matrix<float, 4> ()
{
}

Matrix4::Matrix4 (const Matrix<float, 4>& m)
  : Matrix<float, 4> (m)
{
}

Matrix4::Matrix4 (const Matrix4& m)
  : Matrix<float, 4> (m)
{
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

  (*this)[0][0] = aa + bb - cc - dd;
  (*this)[0][1] = 2 * (bc - ad);
  (*this)[0][2] = 2 * (bd + ac);
  (*this)[0][3] = 0;
  (*this)[1][0] = 2 * (bc + ad);
  (*this)[1][1] = aa - bb + cc - dd;
  (*this)[1][2] = 2 * (cd - ab);
  (*this)[1][3] = 0;
  (*this)[2][0] = 2 * (bd - ac);
  (*this)[2][1] = 2 * (cd + ab);
  (*this)[2][2] = aa - bb - cc + dd;
  (*this)[2][3] = 0;
  (*this)[3][0] = 0;
  (*this)[3][1] = 0;
  (*this)[3][2] = 0;
  (*this)[3][3] = 1;
}

const Matrix4& Matrix4::operator= (const Matrix<float, 4>& m)
{
  Matrix<float, 4>::operator= (m);
  return *this;
}

const Matrix4& Matrix4::operator= (const Matrix4& m)
{
  Matrix<float, 4>::operator= (m);
  return *this;
}

const Matrix4& Matrix4::translate (const Vector3& v)
{
  return translate (v[0], v[1], v[2]);
}

const Matrix4& Matrix4::translate (const Vector4& v)
{
  return translate (v[0], v[1], v[2]);
}

const Matrix4& Matrix4::translate (float x, float y, float z)
{
  Matrix4 tm;

  tm[3][0] = x;
  tm[3][1] = y;
  tm[3][2] = z;

  *this *= tm;

  return *this;
}

Matrix4 Matrix4::translated (const Vector3& v) const
{
  Matrix4 ret = *this;

  return ret.translate (v);
}

Matrix4 Matrix4::translated (const Vector4& v) const
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
  *this *= rm;
  return *this;
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

  rm[0][0] = x;
  rm[1][1] = y;
  rm[2][2] = z;

  *this *= rm;

  return *this;
}

Matrix4 Matrix4::scaled (float x, float y, float z) const
{
  Matrix4 ret = *this;

  return ret.scale (x, y, z);
}

const Matrix4& Matrix4::invert ()
{
  /* blockwise inversion */
  Matrix2 A ((*this)[0][0], (*this)[0][1], (*this)[1][0], (*this)[1][1]);
  Matrix2 B ((*this)[0][2], (*this)[0][3], (*this)[1][2], (*this)[1][3]);
  Matrix2 C ((*this)[2][0], (*this)[2][1], (*this)[3][0], (*this)[3][1]);
  Matrix2 D ((*this)[2][2], (*this)[2][3], (*this)[3][2], (*this)[3][3]);

  A.invert ();

  Matrix2 schur = C;
  schur *= A;
  schur *= B;
  schur = D - schur;
  schur.invert ();

  Matrix2 CA = C * A;
  Matrix2 AB = A * B;
  //Matrix2 schur = D - C * AB;
  //schur.invert ();

  Matrix2 n_A = A + AB * schur * CA;
  Matrix2 n_B = -AB * schur;
  Matrix2 n_C = -schur * CA;

  (*this)[0][0] = n_A[0][0];
  (*this)[0][1] = n_A[0][1];
  (*this)[0][2] = n_B[0][0];
  (*this)[0][3] = n_B[0][1];
  (*this)[1][0] = n_A[1][0];
  (*this)[1][1] = n_A[1][1];
  (*this)[1][2] = n_B[1][0];
  (*this)[1][3] = n_B[1][1];
  (*this)[2][0] = n_C[0][0];
  (*this)[2][1] = n_C[0][1];
  (*this)[2][2] = schur[0][0];
  (*this)[2][3] = schur[0][1];
  (*this)[3][0] = n_C[1][0];
  (*this)[3][1] = n_C[1][1];
  (*this)[3][2] = schur[1][0];
  (*this)[3][3] = schur[1][1];

  return *this;
}

Matrix4 Matrix4::inverted () const
{
  Matrix4 r = *this;

  r.invert ();

  return r;
}

void Matrix4::look_at (const Vector3& eye_location,
                       const Vector3& eye_direction,
                       const Vector3& up_vector)
{
  make_identity ();

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
  make_identity ();

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
