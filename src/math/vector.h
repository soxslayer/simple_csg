#ifndef __VECTOR__
#define __VECTOR__
#include <iostream>

#include <cmath>
#include <initializer_list>
#include <memory>
#include <ostream>
#include <type_traits>

#include "util/zero_one.h"

namespace Math
{

template<typename T, unsigned int D, typename X, typename Z>
class VectorBase
{
public:
  VectorBase ()
  {
    reset (Z::zero ());
  }

  VectorBase (const std::initializer_list<T>& list)
  {
    auto list_iter = list.begin ();
    auto list_end = list.end ();

    for (unsigned int i = 0; i < D && list_iter != list_end; ++i, ++list_iter)
      _data[i] = *list_iter;
  }

  virtual ~VectorBase ()
  {
  }

  const X& operator+= (const X& v)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] += v[i];
    return *static_cast<const X*> (this);
  }

  const X& operator+= (T t)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] += t;
    return *static_cast<const X*> (this);
  }

  const X& operator-= (const X& v)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] -= v[i];
    return *static_cast<const X*> (this);
  }

  const X& operator-= (const T& t)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] -= t;
    return *static_cast<const X*> (this);
  }

  const X& operator*= (const T& t)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] *= t;
    return *static_cast<const X*> (this);
  }

  const X& operator/= (const T& t)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] /= t;
    return *static_cast<const X*> (this);
  }

  X operator+ (const X& v) const
  {
    X r = *static_cast<const X*> (this);
    r += v;
    return r;
  }

  X operator+ (const T& t) const
  {
    X r = *static_cast<const X*> (this);
    r += t;
    return r;
  }

  X operator- (const X& v) const
  {
    X r = *static_cast<const X*> (this);
    r -= v;
    return r;
  }

  X operator- (const T& t) const
  {
    X r = *static_cast<const X*> (this);
    r -= t;
    return r;
  }

  X operator* (const T& t) const
  {
    X r = *static_cast<const X*> (this);
    r *= t;
    return r;
  }

  X operator/ (const T& t) const
  {
    X r = *static_cast<const X*> (this);
    r /= t;
    return r;
  }

  const T& operator[] (unsigned int i) const
  {
    return _data[i];
  }

  T& operator[] (unsigned int i)
  {
    return _data[i];
  }

  bool operator== (const X& v) const
  {
    for (unsigned int i = 0; i < D; ++i) {
      if (_data[i] != v[i])
        return false;
    }
    return true;
  }

  bool operator!= (const X& v) const
  {
    return !(*this == v);
  }

  X operator- () const
  {
    X r = *static_cast<const X*> (this);
    for (unsigned int i = 0; i < D; ++i)
      r[i] *= -1;
    return r;
  }

  unsigned size () const { return D; }

  T length () const
  {
    T l = 0;

    for (unsigned int i = 0; i < D; ++i)
      l += _data[i] * _data[i];

    return sqrt (l);
  }

  T delta (const X& v) const
  {
    return (v - (*this)).length ();
  }

  T pt_length () const
  {
    T l = 0;

    for (unsigned int i = 0; i < D - 1; ++i)
      l += _data[i] * _data[i];

    return sqrt (l);
  }

  const X& normalize ()
  {
    double l = length ();
    *this /= l;
    return *static_cast<const X*> (this);
  }

  X normalized () const
  {
    X r = *static_cast<const X*> (this);
    r.normalize();
    return r;
  }

  T dotp (const X& v) const
  {
    T dp = 0;

    for (int i = 0; i < D; ++i)
      dp += (*this)[i] * v[i];

    return dp;
  }

  const X& reset (const T& fill)
  {
    for (unsigned int i = 0; i < D; ++i)
      _data[i] = fill;
    return *static_cast<const X*> (this);
  }

  X vector ()
  {
    X ret;

    ret[D - 1] = Z::zero ();

    return X ();
  }

  X point ()
  {
    X ret;

    ret[D - 1] = Z::one ();

    return ret;
  }

  T* data () { return _data; }
  const T* data () const { return _data; }

  static X zero ()
  {
    X r;

    for (int i = 0; i < D; ++i)
      r[i] = Z::zero ();

    return r;
  }

  static X one ()
  {
    X r;

    for (int i = 0; i < D; ++i)
      r[i] = Z::one ();

    return r;
  }

  static X axis (unsigned int a)
  {
    X ret;

    if (a < D)
      ret[a] = 1;

    return ret;
  }

private:
  T _data[D];
};

template<typename T, unsigned int D, typename X, typename Z>
std::ostream& operator<< (std::ostream& os, const VectorBase<T, D, X, Z>& v)
{
  bool first = true;

  os << '[';
  for (unsigned int i = 0; i < v.size (); ++i) {
    if (!first)
      os << ',';

    os << v[i];

    first = false;
  }
  os << ']';

  return os;
}

template<typename T, unsigned int D, typename Z = Util::ZeroOne<T>>
class Vector : public VectorBase<T, D, Vector<T, D, Z>, Z>
{
public:
  using VectorBase<T, D, Vector<T, D, Z>, Z>::VectorBase;
};

template<typename T, typename Z = Util::ZeroOne<T>>
using Vector2 = Vector<T, 2>;

template<typename T, typename Z = Util::ZeroOne<T>>
class Vector3 : public VectorBase<T, 3, Vector3<T, Z>, Z>
{
public:
  using VectorBase<T, 3, Vector3<T, Z>, Z>::VectorBase;

  Vector3<T, Z> cross_product (const Vector3<T, Z>& v) const
  {
    Vector3<T, Z> ret;

    ret[0] = (*this)[1] * v[2] - (*this)[2] * v[1];
    ret[1] = (*this)[2] * v[0] - (*this)[0] * v[2];
    ret[2] = (*this)[0] * v[1] - (*this)[1] * v[0];

    return ret;
  }

  T angle_between (const Vector3<T, Z>& v) const
  {
    T m_l = this->length ();
    T v_l = v.length ();
    T dp = this->dotp (v);

    /* XXX Not template friendly */
    return acosf (dp / m_l / v_l) * 180 / M_PI;
  }
};

template<typename T, typename Z = Util::ZeroOne<T>>
using Vector4 = Vector<T, 4>;

} /* namespace Math */

#endif /* __VECTOR__ */
