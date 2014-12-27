#ifndef __MATRIX2__
#define __MATRIX2__

#include <ostream>

namespace Math2
{

class Matrix2
{
public:
  Matrix2 (float a11, float a12, float a21, float a22);
  Matrix2 (const Matrix2& m);

  float operator[] (int idx) const { return _d[idx]; }
  Matrix2 operator- () const;

  const Matrix2& operator*= (const Matrix2& m);
  const Matrix2& operator*= (float f);
  const Matrix2& operator-= (const Matrix2& m);
  const Matrix2& operator+= (const Matrix2& m);

  Matrix2 operator* (const Matrix2& m);
  Matrix2 operator* (float f);
  Matrix2 operator- (const Matrix2& m);
  Matrix2 operator+ (const Matrix2& m);

  const Matrix2& invert ();
  Matrix2 inverted () const;

  float determinant () const;

private:
  float _d[4];
};

std::ostream& operator<< (std::ostream& os, const Matrix2& m);

} /* namespace Math */

#endif /* __MATRIX2__ */
