#ifndef __VECTOR3__
#define __VECTOR3__

#include <ostream>

#include "vector4.h"

namespace Math
{

class Vector3
{
public:
  float x;
  float y;
  float z;

  Vector3 ();
  Vector3 (float x, float y, float z);
  Vector3 (const Vector3& v);
  const Vector3& operator= (const Vector3& v);

  void reset ();
  float length () const;
  float dot_product (const Vector3& v) const;
  Vector3 cross_product (const Vector3& v) const;
  const Vector3& normalize ();
  Vector3 normalized () const;
  float angle_between (const Vector3& v) const;

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
  Vector3 operator- () const;

  static const Vector3 x_axis;
  static const Vector3 y_axis;
  static const Vector3 z_axis;
};

std::ostream& operator<< (std::ostream& s, const Vector3& v);

} /* namespace Math */

#endif /* __VECTOR3__ */
