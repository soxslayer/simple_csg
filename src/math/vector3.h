#ifndef __VECTOR3__
#define __VECTOR3__

#include "vector.h"

namespace Math
{

#if 0
class Vector3 : public Vector<float, 3>
{
public:
  Vector3 ();
  Vector3 (float x, float y, float z);
  Vector3 (const Vector<float, 3>& v);
  Vector3 (const Vector3& v);
  const Vector3& operator= (const Vector<float, 3>& v);
  const Vector3& operator= (const Vector3& v);

  float dot_product (const Vector<float, 3>& v) const;
  Vector3 cross_product (const Vector<float, 3>& v) const;
  float angle_between (const Vector<float, 3>& v) const;

  static const Vector3 x_axis;
  static const Vector3 y_axis;
  static const Vector3 z_axis;
};
#endif

namespace Vector3
{



} /* namespace Vector3 */

} /* namespace Math */

#endif /* __VECTOR3__ */
