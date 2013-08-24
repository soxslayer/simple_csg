#ifndef __VECTOR3__
#define __VECTOR3__

#include <ostream>

namespace Math
{

class Vector3
{
public:
  Vector3 ();
  Vector3 (float x, float y, float z);
  Vector3 (const Vector3& v);
  const Vector3& operator= (const Vector3& v);

  float dot_product (const Vector3& v);
  Vector3 cross_product (const Vector3& v);
  const Vector3& normalize ();
  Vector3 normalized () const;
  float length () const;
  void reset ();

  const Vector3& operator+= (const Vector3& v);
  const Vector3& operator-= (const Vector3& v);
  const Vector3& operator*= (float d);
  const Vector3& operator/= (float d);

  Vector3 operator+ (const Vector3& v) const;
  Vector3 operator- (const Vector3& v) const;
  Vector3 operator* (float d) const;
  Vector3 operator/ (float d) const;

  bool operator== (const Vector3& v) const;
  bool operator!= (const Vector3& v) const;

  float x;
  float y;
  float z;

  static const Vector3 x_axis;
  static const Vector3 y_axis;
  static const Vector3 z_axis;
};

std::ostream& operator<< (std::ostream& s, const Vector3& v);

} /* namespace Math */

#endif /* __VECTOR3__ */
