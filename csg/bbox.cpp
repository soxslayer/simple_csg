#include "bbox.h"
#include "math/util.h"

namespace CSG
{

BBox::BBox ()
  : lb (_lb), ub (_ub)
{
}

BBox::BBox (const Math::Vector3& center, float dim_x,
            float dim_y, float dim_z)
  : lb (_lb), ub (_ub),
    _lb (center.x - dim_x / 2, center.y - dim_y / 2, center.z - dim_z / 2),
    _ub (center.x + dim_x / 2, center.y + dim_y / 2, center.z + dim_z / 2)
{
}

BBox::BBox (float dim_x, float dim_y, float dim_z)
  : lb (_lb), ub (_ub),
    _lb (-dim_x / 2, -dim_y / 2, -dim_z / 2),
    _ub (dim_x / 2, dim_y / 2, dim_z / 2)
{
}

BBox::BBox (const Math::Vector3& v1, const Math::Vector3& v2)
  : lb (_lb), ub (_ub), _lb (v1), _ub (v2)
{
}

BBox::BBox (const BBox& b)
  : lb (_lb), ub (_ub), _lb (b.lb), _ub (b.ub)
{
}

const BBox& BBox::operator= (const BBox& b)
{
  if (&b != this) {
    _lb = b.lb;
    _ub = b.ub;
  }

  return *this;
}

bool BBox::intersects (const Math::Vector3& v) const
{
  return v.x >= lb.x && v.y >= lb.y && v.z >= lb.z
         && v.x <= ub.x && v.y <= ub.y && v.z <= ub.z
         ? true
         : false;
}

Math::Vector3 BBox::size () const
{
  return ub - lb;
}

Math::Vector3 BBox::center () const
{
  return lb + (ub - lb) / 2;
}

BBox BBox::intersection (const BBox& b1, const BBox& b2)
{
  Math::Vector3 min_pt;
  Math::Vector3 max_pt;
  float min1;
  float max1;
  float min2;
  float max2;

  min1 = b1.lb.x;
  max1 = b1.ub.x;
  min2 = b2.lb.x;
  max2 = b2.ub.x;

  if (Math::f_between (min1, min2, max2))
    min_pt.x = min1;
  else if (Math::f_between (min2, min1, max1))
    min_pt.x = min2;
  else
    return BBox ();

  if (Math::f_between (max1, min2, max2))
    max_pt.x = max1;
  else if (Math::f_between (max2, min1, max1))
    max_pt.x = max2;
  else
    return BBox ();

  min1 = b1.lb.y;
  max1 = b1.ub.y;
  min2 = b2.lb.y;
  max2 = b2.ub.y;

  if (Math::f_between (min1, min2, max2))
    min_pt.y = min1;
  else if (Math::f_between (min2, min1, max1))
    min_pt.y = min2;
  else
    return BBox ();

  if (Math::f_between (max1, min2, max2))
    max_pt.y = max1;
  else if (Math::f_between (max2, min1, max1))
    max_pt.y = max2;
  else
    return BBox ();

  min1 = b1.lb.z;
  max1 = b1.ub.z;
  min2 = b2.lb.z;
  max2 = b2.ub.z;

  if (Math::f_between (min1, min2, max2))
    min_pt.z = min1;
  else if (Math::f_between (min2, min1, max1))
    min_pt.z = min2;
  else
    return BBox ();

  if (Math::f_between (max1, min2, max2))
    max_pt.z = max1;
  else if (Math::f_between (max2, min1, max1))
    max_pt.z = max2;
  else
    return BBox ();

  return BBox (min_pt, max_pt);
}

BBox BBox::difference (const BBox& b1, const BBox& b2)
{
  return BBox ();
}

BBox BBox::combination (const BBox& b1, const BBox& b2)
{
  Math::Vector3 min_pt;
  Math::Vector3 max_pt;
  float pt1;
  float pt2;

  pt1 = b1.lb.x;
  pt2 = b2.lb.x;
  if (pt1 < pt2)
    min_pt.x = pt1;
  else
    min_pt.x = pt2;

  pt1 = b1.ub.x;
  pt2 = b2.ub.x;
  if (pt1 > pt2)
    max_pt.x = pt1;
  else
    max_pt.x = pt2;

  pt1 = b1.lb.y;
  pt2 = b2.lb.y;
  if (pt1 < pt2)
    min_pt.y = pt1;
  else
    min_pt.y = pt2;

  pt1 = b1.ub.y;
  pt2 = b2.ub.y;
  if (pt1 > pt2)
    max_pt.y = pt1;
  else
    max_pt.y = pt2;

  pt1 = b1.lb.z;
  pt2 = b2.lb.z;
  if (pt1 < pt2)
    min_pt.z = pt1;
  else
    min_pt.z = pt2;

  pt1 = b1.ub.z;
  pt2 = b2.ub.z;
  if (pt1 > pt2)
    max_pt.z = pt1;
  else
    max_pt.z = pt2;

  return BBox (min_pt, max_pt);
}

std::ostream& operator<< (std::ostream& s, const BBox& bb)
{
  s << bb.lb << ' ' << bb.ub;

  return s;
}

} /* namespace CSG */
