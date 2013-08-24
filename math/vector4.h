#ifndef VECTOR4_H
#define VECTOR4_H

namespace Math
{

class Vector3;

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

  const Vector4& operator+= (const Vector4& v);
  const Vector4& operator-= (const Vector4& v);
  const Vector4& operator*= (float d);
  const Vector4& operator*= (const Vector4& v);
  const Vector4& operator/= (float d);

  Vector4 operator+ (const Vector4& v) const;
  Vector4 operator- (const Vector4& v) const;
  Vector4 operator* (float d) const;
  Vector4 operator* (const Vector4& v) const;
  Vector4 operator/ (float d) const;

  bool operator== (const Vector4& v) const;
  bool operator!= (const Vector4& v) const;

  float x;
  float y;
  float z;
  float w;
};

} /* namespace Math */

#endif /* VECTOR4_H */
