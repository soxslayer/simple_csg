#ifndef VECTOR4_H
#define VECTOR4_H

#include <ostream>

namespace Math
{

class Vector3;
class Matrix4;

class Vector4
{
public:
  Vector4 ();
  Vector4 (float x, float y, float z, float w);
  Vector4 (const Vector3& v);
  Vector4 (const Vector4& v);
  const Vector4& operator= (const Vector4& v);

  const Vector4& normalize ();
  Vector4 normalized ();
  float length () const;
  void reset ();
  float* data () { return _data; }
  const float* data () const { return _data; }

  const Vector4& operator+= (const Vector4& v);
  const Vector4& operator-= (const Vector4& v);
  const Vector4& operator*= (float d);
  const Vector4& operator*= (const Vector4& v);
  const Vector4& operator*= (const Matrix4& m);
  const Vector4& operator/= (float d);

  Vector4 operator+ (const Vector4& v) const;
  Vector4 operator- (const Vector4& v) const;
  Vector4 operator* (float d) const;
  Vector4 operator* (const Vector4& v) const;
  Vector4 operator* (const Matrix4& m) const;
  Vector4 operator/ (float d) const;

  bool operator== (const Vector4& v) const;
  bool operator!= (const Vector4& v) const;

  float& x;
  float& y;
  float& z;
  float& w;

  static Vector4 x_axis;
  static Vector4 y_axis;
  static Vector4 z_axis;
  static Vector4 w_axis;

private:
  float _data[4];
};

std::ostream& operator<< (std::ostream& s, const Vector4& v);

} /* namespace Math */

#endif /* VECTOR4_H */
