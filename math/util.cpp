#include <math.h>

#include "util.h"

namespace Math
{

float deg_to_rad (float deg)
{
  return deg * M_PI / 180.0;
}

bool f_between (float v, float l, float h)
{
  return v >= l && v <= h;
}

float f_abs (float d)
{
  if (d < 0)
    d *= -1;

  return d;
}

float f_clamp (float v, float l, float h)
{
  if (v < l) return l;
  if (v > h) return h;

  return v;
}

float f_interpolate (float x, float x0, float y0, float x1, float y1)
{
  return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

} /* namespace Math */
