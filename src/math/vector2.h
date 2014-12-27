#ifndef __VECTOR2__
#define __VECTOR2__

#include <ostream>

#include "vector.h"

namespace Math
{

class Vector2 : public Vector<float, 2>
{
public:
  Vector2 ();
  Vector2 (float x, float y);
  Vector2 (const Vector<float, 2>& v);
  Vector2 (const Vector2& v);
  const Vector2& operator= (const Vector<float, 2>& v);
  const Vector2& operator= (const Vector2& v);

  static Vector2 x_axis;
  static Vector2 y_axis;
};

} /* namespace Math */

#endif /* __VECTOR2__ */
