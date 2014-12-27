#ifndef __QUATERNION__
#define __QUATERNION__

#include <ostream>

#include "vector.h"

namespace Math
{

template<typename T>
class Quaternion
{
public:
  Quaternion ()
  {
    _data[0] = 0;
    _data[1] = 0;
    _data[2] = 0;
    _data[3] = 1;
  }

  Quaternion (const Vector3<T>& axis, const T& angle)
  {
    T alpha_2 = angle * M_PI / 360;
    T s_alpha_2 = sin (alpha_2);

    _data[0] = axis[0] * s_alpha_2;
    _data[1] = axis[1] * s_alpha_2;
    _data[2] = axis[2] * s_alpha_2;
    _data[3] = cos (alpha_2);
  }

  Quaternion (const Vector4<T>& axis, const T& angle)
  {
    T alpha_2 = angle * M_PI / 360;
    T s_alpha_2 = sin (alpha_2);

    _data[0] = axis[0] * s_alpha_2;
    _data[1] = axis[1] * s_alpha_2;
    _data[2] = axis[2] * s_alpha_2;
    _data[3] = cos (alpha_2);
  }

  const Quaternion<T>& conjugate ()
  {
    _data[0] *= -1;
    _data[1] *= -1;
    _data[2] *= -1;

    return *this;
  }

  Quaternion<T> conjugated () const
  {
    Quaternion ret = *this;

    return ret.conjugate ();
  }

  T& operator[] (unsigned int i)
  {
    return _data[i];
  }

  const T& operator[] (unsigned int i) const
  {
    return _data[i];
  }

  const Quaternion<T>& operator*= (const Quaternion<T>& q)
  {
    Vector4<T> n = { _data[3] * q[0] + _data[0] * q[3]
                       + _data[1] * q[2] - _data[2] * q[1],
                     _data[3] * q[1] + _data[1] * q[3]
                       - _data[0] * q[2] + _data[2] * q[0],
                     _data[3] * q[2] + _data[2] * q[3]
                       + _data[0] * q[1] - _data[1] * q[0],
                     _data[3] * q[3] - _data[0] * q[0]
                       - _data[1] * q[1] - _data[2] * q[2]};
    _data = n;
    return *this;
  }

  Quaternion<T> operator* (const Quaternion<T>& q)
  {
    Quaternion<T> ret = *this;

    return ret *= q;
  }

  friend std::ostream& operator<< (std::ostream& os, const Quaternion& q)
  {
    return os << q._data;
  }

private:
  Vector4<T> _data;
};

} /* namespace Math */

#endif /* __QUATERNION__ */
