#include "brect.h"

namespace CSG
{

BRect::BRect ()
  : lb (_lb), ub (_ub)
{
}

BRect::BRect (const Math::Vector2& center, float dim_x, float dim_y)
  : lb (_lb), ub (_ub),
    _lb (center.x - dim_x / 2, center.y - dim_y / 2),
    _ub (center.x + dim_x / 2, center.y + dim_y / 2)
{
}

BRect::BRect (float dim_x, float dim_y)
  : lb (_lb), ub (_ub),
    _lb (-dim_x / 2, -dim_y / 2),
    _ub (dim_x / 2, dim_y / 2)
{
}

BRect::BRect (const Math::Vector2& v1, const Math::Vector2& v2)
  : lb (_lb), ub (_ub), _lb (v1), _ub (v2)
{
}

BRect::BRect (const BRect& b)
  : lb (_lb), ub (_ub), _lb (b.lb), _ub (b.ub)
{
}

const BRect& BRect::operator= (const BRect& b)
{
  if (&b != this) {
    _lb = b.lb;
    _ub = b.ub;
  }

  return *this;
}

bool BRect::intersects (const Math::Vector2& v) const
{
  return v.x >= lb.x && v.y >= lb.y
         && v.x <= ub.x && v.y <= ub.y
         ? true
         : false;
}

Math::Vector2 BRect::size () const
{
  return ub - lb;
}

Math::Vector2 BRect::center () const
{
  return lb + (ub - lb) / 2;
}

} /* namespace CSG */
