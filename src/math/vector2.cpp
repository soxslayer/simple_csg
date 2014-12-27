#include <math.h>

#include "vector2.h"

namespace Math
{

Vector2::Vector2 ()
{
}

Vector2::Vector2 (float x, float y)
  : Vector<float, 2> ()
{
  (*this)[0] = x;
  (*this)[1] = y;
}

Vector2::Vector2 (const Vector<float, 2>& v)
  : Vector<float, 2> (v)
{
}

Vector2::Vector2 (const Vector2& v)
  : Vector<float, 2> (v)
{
}

const Vector2& Vector2::operator= (const Vector<float, 2>& v)
{
  Vector<float, 2>::operator= (v);
  return *this;
}

const Vector2& Vector2::operator= (const Vector2& v)
{
  Vector<float, 2>::operator= (v);
  return *this;
}

Vector2 Vector2::x_axis (1, 0);
Vector2 Vector2::y_axis (0, 1);

}
