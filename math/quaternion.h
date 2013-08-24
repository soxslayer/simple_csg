#ifndef __QUATERNION__
#define __QUATERNION__

#include <ostream>

#include "vector4.h"

namespace Math
{

class Vector3;

class Quaternion
{
public:
  Quaternion ();
  Quaternion (const Vector3& axis, float angle);
  Quaternion (const Vector4& v);
  const Quaternion& operator= (const Quaternion& q);

  const Quaternion& conjugate ();
  Quaternion conjugated () const;

  const Quaternion& operator*= (const Quaternion& q);
  Quaternion operator* (const Quaternion& q);

  float x;
  float y;
  float z;
  float w;
};

std::ostream& operator<< (std::ostream& s, const Quaternion& q);

} /* namespace Math */

#endif /* __QUATERNION__ */
