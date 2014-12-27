#include "brect.h"
#include "pixel.h"
#include "util/debug.h"

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

BRect::BRect (const Pixel& p)
  : lb (_lb), ub (_ub),
    _lb (p.center - p.size / 2), _ub (p.center + p.size / 2)
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

BRect BRect::combination (const BRect& r1, const BRect& r2)
{
  Math::Vector2 min_pt;
  Math::Vector2 max_pt;
  float pt1;
  float pt2;

  pt1 = r1.lb.x;
  pt2 = r2.lb.x;
  if (pt1 < pt2)
    min_pt.x = pt1;
  else
    min_pt.x = pt2;

  pt1 = r1.ub.x;
  pt2 = r2.ub.x;
  if (pt1 > pt2)
    max_pt.x = pt1;
  else
    max_pt.x = pt2;

  pt1 = r1.lb.y;
  pt2 = r2.lb.y;
  if (pt1 < pt2)
    min_pt.y = pt1;
  else
    min_pt.y = pt2;

  pt1 = r1.ub.y;
  pt2 = r2.ub.y;
  if (pt1 > pt2)
    max_pt.y = pt1;
  else
    max_pt.y = pt2;

  DBG(r1);
  DBG(r2);
  DBG(min_pt);
  DBG(max_pt);
  return BRect (min_pt, max_pt);
}

} /* namespace CSG */

ostream& operator<< (ostream& os, const CSG::BRect& b)
{
  os << b.lb << ' ' << b.ub;

  return os;
}
