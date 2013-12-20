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

void Surface::traverse (TraverseCallback callback, double resolution) const
{
  Pixel p (_brect);

  visit (callback, p, resolution);
}

float Surface::density (const Math::Vector2& v) const
{
  return density (v.x, v.y);
}

bool Surface::visit (TraverseCallback callback, const Pixel& pix,
                     double min_size) const
{
  return false;
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

} /* namespace CSG */
