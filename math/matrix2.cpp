#include "matrix2.h"

using namespace std;

namespace Math
{

Matrix2::Matrix2 (float a11, float a12, float a21, float a22)
{
  _d[0] = a11;
  _d[1] = a12;
  _d[2] = a21;
  _d[3] = a22;
}

Matrix2::Matrix2 (const Matrix2& m)
{
  _d[0] = m[0];
  _d[1] = m[1];
  _d[2] = m[2];
  _d[3] = m[3];
}

Matrix2 Matrix2::operator- () const
{
  Matrix2 r = *this;

  r *= -1;

  return r;
}

const Matrix2& Matrix2::operator*= (const Matrix2& m)
{
  float d[4];

  d[0] = _d[0] * m[0] + _d[1] * m[2];
  d[1] = _d[0] * m[1] + _d[1] * m[3];
  d[2] = _d[2] * m[0] + _d[3] * m[2];
  d[3] = _d[2] * m[1] + _d[3] * m[3];

  _d[0] = d[0];
  _d[1] = d[1];
  _d[2] = d[2];
  _d[3] = d[3];

  return *this;
}

const Matrix2& Matrix2::operator*= (float f)
{
  _d[0] *= f;
  _d[1] *= f;
  _d[2] *= f;
  _d[3] *= f;

  return *this;
}

const Matrix2& Matrix2::operator-= (const Matrix2& m)
{
  _d[0] -= m[0];
  _d[1] -= m[1];
  _d[2] -= m[2];
  _d[3] -= m[3];

  return *this;
}

const Matrix2& Matrix2::operator+= (const Matrix2& m)
{
  _d[0] += m[0];
  _d[1] += m[1];
  _d[2] += m[2];
  _d[3] += m[3];

  return *this;
}

Matrix2 Matrix2::operator* (const Matrix2& m)
{
  Matrix2 r = *this;

  r *= m;

  return r;
}

Matrix2 Matrix2::operator* (float f)
{
  Matrix2 r = *this;

  r *= f;

  return r;
}

Matrix2 Matrix2::operator- (const Matrix2& m)
{
  Matrix2 r = *this;

  r -= m;

  return r;
}

Matrix2 Matrix2::operator+ (const Matrix2& m)
{
  Matrix2 r = *this;

  r += m;

  return r;
}

const Matrix2& Matrix2::invert ()
{
  float a = _d[0];
  float d = _d[3];
  float det = determinant ();

  _d[0] = d / det;
  _d[1] = -_d[1] / det;
  _d[2] = -_d[2] / det;
  _d[3] = a / det;

  return *this;
}

Matrix2 Matrix2::inverted () const
{
  Matrix2 r = *this;

  r.invert ();

  return r;
}

float Matrix2::determinant () const
{
  return _d[0] * _d[3] - _d[1] * _d[2];
}

ostream& operator<< (ostream& os, const Matrix2& m)
{
  os << "[[ " << m[0] << ", " << m[1] << "]" << endl;
  os << " [ " << m[2] << ", " << m[3] << "]]";

  return os;
}

} /* namespace Math */
