#ifndef VECTOR4_H
#define VECTOR4_H

#include "vector.h"

namespace Math
{

#if 0
class Vector3;
class Matrix4;

class Vector4 : public Vector<float, 4>
{
public:
  Vector4 ();
  Vector4 (float x, float y, float z, float w);
  Vector4 (const Vector3& v);
  Vector4 (const Vector<float, 4>& v);
  Vector4 (const Vector4& v);
  const Vector4& operator= (const Vector<float, 4>& v);
  const Vector4& operator= (const Vector4& v);

  const Vector4& operator*= (const Matrix4& m);
  Vector4 operator* (const Matrix4& m) const;

  static const Vector4 x_axis;
  static const Vector4 y_axis;
  static const Vector4 z_axis;
  static const Vector4 w_axis;
};
#endif

namespace Vector4
{

template<typename T>
const Vector<T, 4>& operator*= (const Vector<T, 4>& v, const Matrix<T, 4>& m)
{
  T n_x = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[8] + v[3] * m[12];
  T n_y = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[9] + v[3] * m[13];
  T n_z = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[10] + v[3] * m[14];
  T n_w = v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[11] + v[3] * m[15];

  v.set (n_x, n_y, n_z, n_w);

  return v;
}

} /* namespace Vector4 */

} /* namespace Math */

#endif /* VECTOR4_H */
