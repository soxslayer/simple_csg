#ifndef __UTIL__
#define __UTIL__

namespace Math
{

template<typename T>
T deg_to_rad(T deg)
{
  return deg * static_cast<T>(M_PI) / static_cast<T>(180.0);
}

template<typename T>
T interpolate(const T& x, const T& x0, const T& y0,
              const T& x1, const T& y1)
{
  return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

template<typename T>
bool between(const T& v, const T& l, const T& h)
{
  return v >= l && v <= h;
}

template<typename T>
bool in_range(const T& v, const T& p, const T& r)
{
  return between(v, p - r, p + r);
}

template<typename T>
T abs(const T& d)
{
  T ret = d;

  if (d < 0)
    ret *= -1;

  return ret;
}

template<typename T>
T clamp(const T& v, const T& l, const T& h)
{
  if (v < l) return l;
  if (v > h) return h;

  return v;
}

template<typename T>
const T& min(const T& a, const T& b)
{
  return a < b ? a : b;
}

template<typename T>
const T& max(const T& a, const T& b)
{
  return a > b ? a : b;
}

} /* namespace Math */

#endif /* __UTIL__ */
