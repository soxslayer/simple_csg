#ifndef __MATRIX__
#define __MATRIX__

#include <memory>
#include <ostream>
#include <type_traits>

#include "quaternion.h"
#include "vector.h"

namespace Math
{

template<typename Item, unsigned int Dim, typename Derived,
         typename ZeroOneType,
         typename VectorType = Vector<Item, Dim, ZeroOneType>>
class MatrixBase : public VectorBase<VectorType, Dim, Derived, VectorType>
{
public:
  using VectorBase<Vector<Item, Dim, ZeroOneType>, Dim, Derived,
                   Vector<Item, Dim, ZeroOneType>>::VectorBase;

  const Derived& operator*= (const Derived& m)
  {
    *static_cast<Derived*> (this) = *static_cast<const Derived*> (this) * m;
    return *static_cast<const Derived*> (this);
  }

  Derived operator* (const Derived& m) const
  {
    Derived ret;

    for (unsigned int r = 0; r < Dim; ++r) {
      for (unsigned int c = 0; c < Dim; ++c) {
        ret[r][c] = 0;

        for (unsigned int n = 0; n < Dim; ++n)
          ret[r][c] += (*this)[r][n] * m[n][c];
      }
    }

    return ret;
  }

  const Derived& transpose ()
  {
    *static_cast<Derived*> (this) = transposed ();
    return *static_cast<const Derived*> (this);
  }

  Derived transposed () const
  {
    Derived ret;

    for (unsigned int r = 0; r < Dim; ++r) {
      for (unsigned int c = 0; c < Dim; ++c)
        ret[c][r] = (*this)[r][c];
    }

    return ret;
  }

  void make_identity ()
  {
    for (unsigned int r = 0; r < Dim; ++r) {
      for (unsigned int c = 0; c < Dim; ++c)
        (*this)[r][c] = r == c;
    }
  }

  static Derived identity ()
  {
    Derived m;
    m.make_identity ();
    return m;
  }

  void copy_data (Item* dst, size_t size)
  {
    for (unsigned int r = 0; r < Dim; ++r) {
      for (unsigned int c = 0; c < Dim; ++c) {
        if (r * 4 + c > size)
          goto out;

        dst[r * 4 + c] = (*this)[r][c];
      }
    }
  out:
    return;
  }
};

template<typename Item, unsigned int Dim, typename Derived,
         typename ZeroOneType, typename VectorType>
VectorType operator* (const VectorType& v,
                      const MatrixBase<Item, Dim, Derived,
                                       ZeroOneType, VectorType>& m)
{
  VectorType ret;

  for (unsigned int c = 0; c < Dim; ++c) {
    ret[c] = 0;

    for (unsigned int r = 0; r < Dim; ++r)
      ret[c] += v[r] * m[r][c];
  }

  return ret;
}

template<typename Item, unsigned int Dim, typename Derived, typename ZeroOneType,
         typename VectorType>
const VectorType& operator*= (VectorType& v,
                              const MatrixBase<Item, Dim, Derived,
                                               ZeroOneType, VectorType>& m)
{
  v = v * m;

  return v;
}

template<typename Item, unsigned int Dim,
         typename ZeroOneType = Util::ZeroOne<Item>>
class Matrix : public MatrixBase<Item, Dim, Matrix<Item, Dim, ZeroOneType>,
                                 ZeroOneType, Vector<Item, Dim, ZeroOneType>>
{
public:
  using MatrixBase<Item, Dim, Matrix<Item, Dim, ZeroOneType>, ZeroOneType,
                   Vector<Item, Dim, ZeroOneType>>::MatrixBase;
};

template<typename Item, typename ZeroOneType = Util::ZeroOne<Item>>
class Matrix2 : public MatrixBase<Item, 2, Matrix2<Item, ZeroOneType>, ZeroOneType,
                                  Vector2<Item, ZeroOneType>>
{
public:
  using MatrixBase<Item, 2, Matrix2<Item, ZeroOneType>, ZeroOneType,
                   Vector2<Item, ZeroOneType>>::MatrixBase;

  Item determinant () const
  {
    return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
  }

  const Matrix2<Item, ZeroOneType>& invert ()
  {
    Item a = (*this)[0][0];
    Item d = (*this)[1][1];
    Item det = determinant ();

    (*this)[0][0] = d / det;
    (*this)[0][1] = -(*this)[0][1] / det;
    (*this)[1][0] = -(*this)[1][0] / det;
    (*this)[1][1] = a / det;

    return *this;
  }

  Matrix2<Item, ZeroOneType> inverted () const
  {
    Matrix2<Item, ZeroOneType> ret = *this;
    ret.invert ();
    return ret;
  }
};

template<typename Item, typename ZeroOneType = Util::ZeroOne<Item>>
class Matrix4 : public MatrixBase<Item, 4, Matrix4<Item, ZeroOneType>, ZeroOneType,
                                  Vector4<Item, ZeroOneType>>
{
public:
  Matrix4 ()
    : MatrixBase<Item, 4, Matrix4<Item, ZeroOneType>, ZeroOneType,
                 Vector4<Item, ZeroOneType>> ()
  {
    this->make_identity ();
  }

  Matrix4 (const std::initializer_list<Vector<Item, 4>>& list)
    : MatrixBase<Item, 4, Matrix4<Item, ZeroOneType>, ZeroOneType,
                 Vector4<Item, ZeroOneType>> (list)
  { }

  Matrix4 (const Quaternion<Item>& q)
  {
    Item aa = q[3] * q[3];
    Item bb = q[0] * q[0];
    Item cc = q[1] * q[1];
    Item dd = q[2] * q[2];

    Item bc = q[0] * q[1];
    Item ad = q[3] * q[2];
    Item bd = q[0] * q[2];
    Item ac = q[3] * q[1];
    Item cd = q[1] * q[2];
    Item ab = q[3] * q[0];

    (*this)[0][0] = aa + bb - cc - dd;
    (*this)[0][1] = 2 * (bc - ad);
    (*this)[0][2] = 2 * (bd + ac);
    (*this)[0][3] = 0;
    (*this)[1][0] = 2 * (bc + ad);
    (*this)[1][1] = aa - bb + cc - dd;
    (*this)[1][2] = 2 * (cd - ab);
    (*this)[1][3] = 0;
    (*this)[2][0] = 2 * (bd - ac);
    (*this)[2][1] = 2 * (cd + ab);
    (*this)[2][2] = aa - bb - cc + dd;
    (*this)[2][3] = 0;
    (*this)[3][0] = 0;
    (*this)[3][1] = 0;
    (*this)[3][2] = 0;
    (*this)[3][3] = 1;
  }

  const Matrix4<Item, ZeroOneType>& scale (const Item& x, const Item& y,
                                           const Item& z)
  {
    Matrix4<Item, ZeroOneType> rm;

    rm[0][0] = x;
    rm[1][1] = y;
    rm[2][2] = z;

    *this *= rm;

    return *this;
  }

  Matrix4<Item, ZeroOneType> scaled (const Item& x, const Item& y,
                                  const Item& z) const
  {
    Matrix4<Item, ZeroOneType> ret = *this;
    ret.scale (x, y, z);
    return ret;
  }

  const Matrix4<Item, ZeroOneType>& translate (const Item& x, const Item& y,
                                            const Item& z)
  {
    Matrix4<Item, ZeroOneType> tm;

    tm[3][0] = x;
    tm[3][1] = y;
    tm[3][2] = z;

    *this *= tm;

    return *this;
  }

  const Matrix4<Item, ZeroOneType>& translate (const Vector3<Item>& v)
  {
    return translate (v[0], v[1], v[2]);
  }

  const Matrix4<Item, ZeroOneType>& translate (const Vector4<Item>& v)
  {
    return translate (v[0], v[1], v[2]);
  }

  Matrix4<Item, ZeroOneType> translated (const Item& x, const Item& y,
                                      const Item& z) const
  {
    Matrix4<Item, ZeroOneType> ret = *this;
    ret.translate (x, y, z);
    return *this;
  }

  Matrix4<Item, ZeroOneType> translated (const Vector3<Item>& v) const
  {
    return translated (v[0], v[1], v[2]);
  }

  Matrix4<Item, ZeroOneType> translated (const Vector4<Item>& v) const
  {
    return translated (v[0], v[1], v[2]);
  }

  const Matrix4<Item, ZeroOneType>& rotate (const Quaternion<Item>& q)
  {
    Matrix4<Item, ZeroOneType> rm = q;
    *this *= rm;
    return *this;
  }

  const Matrix4<Item, ZeroOneType>& rotate (const Vector3<Item, ZeroOneType>& axis,
                                         const Item& angle)
  {
    return rotate (Quaternion<Item> (axis, angle));
  }

  const Matrix4<Item, ZeroOneType>& invert ()
  {
    Matrix2<Item, ZeroOneType> A ({ { (*this)[0][0], (*this)[0][1] },
                               { (*this)[1][0], (*this)[1][1] } });
    Matrix2<Item, ZeroOneType> B ({ { (*this)[0][2], (*this)[0][3] },
                               { (*this)[1][2], (*this)[1][3] } });
    Matrix2<Item, ZeroOneType> C ({ { (*this)[2][0], (*this)[2][1] },
                               { (*this)[3][0], (*this)[3][1] } });
    Matrix2<Item, ZeroOneType> D ({ { (*this)[2][2], (*this)[2][3] },
                               { (*this)[3][2], (*this)[3][3] } });

    A.invert ();

    Matrix2<Item, ZeroOneType> CA = C * A;
    Matrix2<Item, ZeroOneType> AB = A * B;

    Matrix2<Item, ZeroOneType> schur = D - CA * B;
    schur.invert ();

    Matrix2<Item, ZeroOneType> n_A = A + AB * schur * CA;
    Matrix2<Item, ZeroOneType> n_B = -AB * schur;
    Matrix2<Item, ZeroOneType> n_C = -schur * CA;

    (*this)[0][0] = n_A[0][0];
    (*this)[0][1] = n_A[0][1];
    (*this)[0][2] = n_B[0][0];
    (*this)[0][3] = n_B[0][1];
    (*this)[1][0] = n_A[1][0];
    (*this)[1][1] = n_A[1][1];
    (*this)[1][2] = n_B[1][0];
    (*this)[1][3] = n_B[1][1];
    (*this)[2][0] = n_C[0][0];
    (*this)[2][1] = n_C[0][1];
    (*this)[2][2] = schur[0][0];
    (*this)[2][3] = schur[0][1];
    (*this)[3][0] = n_C[1][0];
    (*this)[3][1] = n_C[1][1];
    (*this)[3][2] = schur[1][0];
    (*this)[3][3] = schur[1][1];

    return *this;
  }

  Matrix4<Item, ZeroOneType> inverted () const
  {
    Matrix4<Item, ZeroOneType> ret = *this;
    ret.invert ();
    return ret;
  }

  const Matrix4<Item, ZeroOneType>& look_at (const Vector3<Item>& eye_location,
                                          const Vector3<Item>& eye_direction,
                                          const Vector3<Item>& up_vector)
  {
    this->make_identity ();

    translate (-eye_location);

    Vector3<Item> axis = eye_location.cross_product (Vector3<Item>::axis (2));
    Item angle = 180 - eye_direction.angle_between (Vector3<Item>::axis (2));

    if (angle != 0)
      rotate (axis, angle);

    angle = up_vector.angle_between (Vector3<Item>::axis (1));
    rotate (Vector3<Item>::axis (2), -angle);

    return *this;
  }

  const Matrix4<Item, ZeroOneType>& ortho_projection (
          const Item& left, const Item& right,
          const Item& bottom, const Item& top,
          const Item& near, const Item& far)
  {
    this->make_identity ();

    translate (-(left + right) / 2, -(top + bottom) / 2, -(far + near) / 2);
    scale (2 / (right - left), 2 / (top - bottom), 2 / (far - near));

    return *this;
  }
};

} /* namespace Math */

#endif /* __MATRIX__ */
