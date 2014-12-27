#include <math.h>

#include "surface.h"
#include "pixel.h"
#include "math/util.h"

namespace CSG
{

Surface::Surface (const BRect& brect)
  : _brect (brect)
{
}

Surface::Surface (float x, float y)
  : _brect (x, y)
{
}

Surface::~Surface()
{
}

void Surface::traverse (TraverseCallback callback, float resolution) const
{
  Pixel p (_brect);

  visit (callback, p, resolution);
}

bool Surface::visit (TraverseCallback callback, const Pixel& pix,
                     float min_size) const
{
  if (!intersects (BRect (pix)))
    return false;

  int num_inside = 0;
  float s2 = pix.size / 2;

  for (int i = 0; i < 4; ++i) {
    Math::Vector2 n_pt = pix.center;

    n_pt.x += i & 1 ? -s2 : s2;
    n_pt.y += i & 2 ? -s2 : s2;

    num_inside += test_point (n_pt) ? 1 : 0;
  }

  bool isec = intersects (pix);

  if (num_inside == 0 && !isec)
    return true;
  else if (num_inside == 4)
    return callback (pix, true);
  else if (num_inside < 8 && pix.size <= min_size)
    return callback (pix, false);

  float s4 = pix.size / 4;

  for (int i = 0; i < 4; ++i) {
    Pixel n_pix = pix;

    n_pix.center.x += i & 1 ? -s4 : s4;
    n_pix.center.y += i & 2 ? -s4 : s4;
    n_pix.size = s2;

    if (!visit (callback, n_pix, min_size))
      return false;
  }

  return true;
}

const Math::Matrix2& Surface::itransform ()
{
#if 0
  int hash = _transform.hash ();

  if (hash != _hash) {
    _itransform = _transform.inverted ();
    _hash = hash;
  }
#endif
}



CircleSurface::CircleSurface (float radius)
  : Surface (radius * 2, radius * 2), _radius (radius)
{
}

float CircleSurface::density (float x, float y) const
{
  return _radius - sqrt (x * x + y * y);
}

bool CircleSurface::intersects (const BRect& b) const
{
  /* center inside box */
  if (b.ub.x > 0 && b.ub.y > 0 && b.lb.x < 0 && b.lb.y < 0)
    return true;

  /* check four corners of box */
  float pt = sqrt (b.lb.x * b.lb.x + b.ub.y * b.ub.y);
  if (pt < _radius)
    return true;

  pt = sqrt (b.ub.x * b.ub.x + b.ub.y * b.ub.y);
  if (pt < _radius)
    return true;

  pt = sqrt (b.ub.x * b.ub.x + b.lb.y * b.lb.y);
  if (pt < _radius)
    return true;

  pt = sqrt (b.lb.x * b.lb.x + b.lb.y * b.lb.y);
  if (pt < _radius)
    return true;

  /* check four sides of box */
  if (b.lb.x < _radius || b.lb.y < _radius
      || b.ub.x < _radius || b.ub.y < _radius)
    return true;

  return false;
}



RectangleSurface::RectangleSurface (float x, float y)
  : Surface (x, y)
{
}

float RectangleSurface::density (float x, float y) const
{
  Math::Vector2 size = _brect.size ();

  float d_x = size.x / 2 - Math::f_abs (x);
  float d_y = size.y / 2 - Math::f_abs (y);

  return Math::f_min (d_x, d_y);
}

bool RectangleSurface::intersects (const BRect& b) const
{
  return !(b.lb.y >= _brect.ub.y && b.ub.y <= _brect.lb.y
           && b.lb.x >= _brect.ub.x && b.ub.x <= _brect.lb.x);
}



UnionSurface::UnionSurface (const Surface& s1, const Surface& s2)
  : Surface (BRect::combination (s1.bounds (), s2.bounds ())),
    _s1 (s1), _s2 (s2)
{
}

float UnionSurface::density (float x, float y) const
{
  return 0.0;
}

bool UnionSurface::intersects (const BRect& b) const
{
  return false;
}

} /* namespace CSG */
