#ifndef __UTIL__
#define __UTIL__

namespace Math
{

float deg_to_rad (float deg);
bool f_between (float v, float l, float h);
float f_abs (float d);
float f_clamp (float v, float l, float h);
float f_interpolate (float x, float x0, float y0, float x1, float y1);

} /* namespace Math */

#endif /* __UTIL__ */
