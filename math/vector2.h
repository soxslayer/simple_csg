#ifndef __VECTOR2__
#define __VECTOR2__

#include <ostream>

namespace Math
{

class Vector2
{
public:
  float x;
  float y;

  Vector2 ();
  Vector2 (float x, float y);
  Vector2 (const Vector2& v);
  const Vector2& operator= (const Vector2& v);

  const Vector2& operator+= (const Vector2& v);
  const Vector2& operator-= (const Vector2& v);
  const Vector2& operator*= (float f);
  const Vector2& operator/= (float f);

  const Vector2 operator+ (const Vector2& v) const;
  const Vector2 operator- (const Vector2& v) const;
  const Vector2 operator* (float f) const;
  const Vector2 operator/ (float f) const;

  float length () const;

  static Vector2 x_axis;
  static Vector2 y_axis;
};

} /* namespace Math */

std::ostream& operator<< (std::ostream& os, const Math::Vector2& v);

#endif /* __VECTOR2__ */
