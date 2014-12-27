#include "bbox.h"
#include "math/util.h"

namespace CSG
{

BBox::BBox()
{
}

BBox::BBox(const Math::Vector4<float>& lb, const Math::Vector4<float>& ub)
  : _lb(lb), _ub(ub)
{
}

BBox::BBox(float dim_x, float dim_y, float dim_z)
  : _lb({ -dim_x / 2, -dim_y / 2, -dim_z / 2, 1 }),
    _ub({ dim_x / 2, dim_y / 2, dim_z / 2, 1 })
{
}

BBox::BBox(const Math::Vector3<float>& center, float dim)
  : _lb({ center[0] - dim / 2, center[1] - dim / 2, center[2] - dim / 2, 1 }),
    _ub({ center[0] + dim / 2, center[1] + dim / 2, center[2] + dim / 2, 1 })
{
}

BBox::BBox(const Math::Vector4<float>& center, float dim)
  : _lb({ center[0] - dim / 2, center[1] - dim / 2, center[2] - dim / 2, 1 }),
    _ub({ center[0] + dim / 2, center[1] + dim / 2, center[2] + dim / 2, 1 })
{
}

BBox::BBox(const BBox& b)
  : _lb(b._lb), _ub(b._ub)
{
  /* ensure bounds are points */
  _lb[3] = 1;
  _ub[3] = 1;
}

BBox::BBox(const BBox& b, const Math::Matrix4<float>& transform)
  : _lb(b._lb), _ub(b._ub)
{
  _lb[3] = 1;
  _ub[3] = 1;

  this->transform(transform);
}

const BBox& BBox::operator=(const BBox& b)
{
  if (&b != this) {
    _lb = b._lb;
    _ub = b._ub;
  }

  return *this;
}

const BBox& BBox::operator&=(const BBox& b)
{
  if (Math::between(b._lb[0], _lb[0], _ub[0]))
    _lb[0] = b._lb[0];
  else if (!Math::between(_lb[0], b._lb[0], b._ub[0])) {
    _lb = _ub;
    return *this;
  }

  if (Math::between(b._lb[1], _lb[1], _ub[1]))
    _lb[1] = b._lb[1];
  else if (!Math::between(_lb[1], b._lb[1], b._ub[1])) {
    _lb = _ub;
    return *this;
  }

  if (Math::between(b._lb[2], _lb[2], _ub[2]))
    _lb[2] = b._lb[2];
  else if (!Math::between(_lb[2], b._lb[2], b._ub[2])) {
    _lb = _ub;
    return *this;
  }

  if (Math::between(b._ub[0], _lb[0], _ub[0]))
    _ub[0] = b._ub[0];
  else if (!Math::between(_ub[0], b._lb[0], b._ub[0])) {
    _ub = _lb;
    return *this;
  }

  if (Math::between(b._ub[1], _lb[1], _ub[1]))
    _ub[1] = b._ub[1];
  else if (!Math::between(_ub[1], b._lb[1], b._ub[1])) {
    _ub = _lb;
    return *this;
  }

  if (Math::between(b._ub[2], _lb[2], _ub[2]))
    _ub[2] = b._ub[2];
  else if (!Math::between(_ub[2], b._lb[2], b._ub[2])) {
    _ub = _lb;
    return *this;
  }

  return *this;
}

BBox BBox::operator&(const BBox& b) const
{
  BBox ret = *this;
  ret &= b;
  return ret;
}

const BBox& BBox::operator|=(const BBox& b)
{
  if (b._lb[0] < _lb[0])
    _lb[0] = b._lb[0];
  if (b._lb[1] < _lb[1])
    _lb[1] = b._lb[1];
  if (b._lb[2] < _lb[2])
    _lb[2] = b._lb[2];
  if (b._ub[0] > _ub[0])
    _ub[0] = b._ub[0];
  if (b._ub[1] > _ub[1])
    _ub[1] = b._ub[1];
  if (b._ub[2] > _ub[2])
    _ub[2] = b._ub[2];

  return *this;
}

BBox BBox::operator|(const BBox& b)
{
  BBox ret = *this;
  ret |= b;
  return ret;
}

BBox::CornerList BBox::corners() const
{
  CornerList ret;

  for (int i = 0; i < 8; ++i) {
    float x = i & 1 ? _ub[0] : _lb[0];
    float y = i & 2 ? _ub[1] : _lb[1];
    float z = i & 4 ? _ub[2] : _lb[2];

    ret.push_back(Math::Vector4<float>({ x, y, z, 1 }));
  }

  return ret;
}

BBox::EdgeList BBox::edges() const
{
  EdgeList ret;

  CornerList corners;

  ret.push_back(std::make_pair(corners[PX_PY_PZ], corners[PX_PY_NZ]));
  ret.push_back(std::make_pair(corners[PX_NY_PZ], corners[PX_NY_NZ]));
  ret.push_back(std::make_pair(corners[NX_PY_PZ], corners[NX_PY_NZ]));
  ret.push_back(std::make_pair(corners[NX_NY_PZ], corners[NX_NY_NZ]));

  ret.push_back(std::make_pair(corners[PX_PY_PZ], corners[PX_NY_PZ]));
  ret.push_back(std::make_pair(corners[PX_PY_NZ], corners[PX_NY_NZ]));
  ret.push_back(std::make_pair(corners[NX_PY_PZ], corners[NX_NY_PZ]));
  ret.push_back(std::make_pair(corners[NX_PY_NZ], corners[NX_NY_NZ]));

  ret.push_back(std::make_pair(corners[PX_PY_PZ], corners[NX_PY_PZ]));
  ret.push_back(std::make_pair(corners[PX_PY_NZ], corners[NX_PY_NZ]));
  ret.push_back(std::make_pair(corners[PX_NY_PZ], corners[NX_NY_PZ]));
  ret.push_back(std::make_pair(corners[PX_NY_NZ], corners[NX_NY_NZ]));

  return ret;
}

bool BBox::contains(const Math::Vector4<float>& v) const
{
  return Math::between(v[0], _lb[0], _ub[0])
         && Math::between(v[1], _lb[1], _ub[1])
         && Math::between(v[2], _lb[2], _ub[2]);
}

Math::Vector3<float> BBox::size() const
{
  return Math::Vector3<float>({ _ub[0] - _lb[0],
                                 _ub[1] - _lb[1],
                                 _ub[2] - _lb[2] });
}

Math::Vector4<float> BBox::center() const
{
  return { (_ub[0] + _lb[0]) / 2, (_ub[1] + _lb[1]) / 2,
           (_ub[2] + _lb[2]) /2, 1 };
}

const BBox& BBox::transform(const Math::Matrix4<float>& transform)
{
  std::vector<Math::Vector4<float>> pts = corners();
  auto pt = pts.begin();
  auto pt_end = pts.end();

  for (; pt != pt_end; ++pt)
    *pt *= transform;

  _lb = _ub = pts[0];

  for (unsigned int i = 1; i < 8; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      if (pts[i][j] < _lb[j])
        _lb[j] = pts[i][j];

      if (pts[i][j] > _ub[j])
        _ub[j] = pts[i][j];
    }
  }

  _lb[3] = 1;
  _ub[3] = 1;

  return *this;
}

std::ostream& operator<<(std::ostream& s, const BBox& b)
{
  s << b._lb << ' ' << b._ub;

  return s;
}

} /* namespace CSG */
