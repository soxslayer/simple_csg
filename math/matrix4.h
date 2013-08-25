#ifndef __MATRIx4__
#define __MATRIx4__

#include <ostream>

#include "vector4.h"

namespace Math
{

class Quaternion;
class Vector3;

class Matrix4
{
public:
  Matrix4 ();
  Matrix4 (const Matrix4& m);
  Matrix4 (const Quaternion& q);
  const Matrix4& operator= (const Matrix4& m);

  const float* data () const { return _d; }
  float* data () { return _d; }
  const Matrix4& set_data (const float* d);

  const float& operator[] (int c) const { return _d[c]; }
  float& operator[] (int c) { return _d[c]; }

  const Matrix4& operator*= (const Matrix4& m);

  Matrix4 operator* (const Matrix4& m) const;
  //Vector4 operator* (const Vector4& v) const;

  const Matrix4& translate (const Vector3& v);
  const Matrix4& translate (float x, float y, float z);
  Matrix4 translated (const Vector3& v) const;
  Matrix4 translated (float x, float y, float z) const;
  const Matrix4& rotate (const Vector3& axis, float angle);
  const Matrix4& rotate (float x, float y, float z, float angle);
  const Matrix4& rotate (const Quaternion& q);
  const Matrix4& rotate (float phi, float theta, float rho);
  Matrix4 rotated (const Vector3& axis, float angle) const;
  Matrix4 rotated (float x, float y, float z, float angle) const;
  Matrix4 rotated (const Quaternion& q) const;
  Matrix4 rotated (float phi, float theta, float rho) const;
  const Matrix4& scale (float x, float y, float z);
  Matrix4 scaled (float x, float y, float z) const;
  const Matrix4& transpose ();
  Matrix4 transposed () const;
  void reset ();
  void look_at (const Vector3& eye_location, const Vector3& eye_direction,
                const Vector3& up_vector);
  void ortho_projection (float left, float right, float bottom, float top,
                         float near, float far);

  static const Matrix4 identity;

private:
  float _d[16];
};

std::ostream& operator<< (std::ostream& s, const Matrix4& m);

} /* namespace Math */

#endif /* __MATRIx4__ */
