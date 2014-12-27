#ifndef __BOX__
#define __BOX__

#include <ostream>
#include <vector>
#include <utility>

#include "math/matrix.h"
#include "math/vector.h"

namespace CSG
{

class BBox
{
public:
  typedef enum
  {
    PX_PY_PZ = 0b111,
    PX_PY_NZ = 0b011,
    PX_NY_PZ = 0b101,
    PX_NY_NZ = 0b001,
    NX_PY_PZ = 0b110,
    NX_PY_NZ = 0b010,
    NX_NY_PZ = 0b100,
    NX_NY_NZ = 0b000
  } Corner;

  typedef std::vector<Math::Vector4<float>> CornerList;
  typedef std::vector<std::pair<Math::Vector4<float>, Math::Vector4<float>>>
      EdgeList;

  BBox();
  BBox(const Math::Vector4<float>& lb, const Math::Vector4<float>& ub);
  BBox(float dim_x, float dim_y, float dim_z);
  BBox(const Math::Vector3<float>& center, float dim);
  BBox(const Math::Vector4<float>& center, float dim);
  BBox(const BBox& b);
  BBox(const BBox& b, const Math::Matrix4<float>& transform);
  const BBox& operator=(const BBox& b);

  const BBox& operator&=(const BBox& b);
  BBox operator&(const BBox& b) const;
  const BBox& operator|=(const BBox& b);
  BBox operator|(const BBox& b);

  CornerList corners() const;
  EdgeList edges() const;
  bool is_singular() const { return _lb == _ub; }
  bool contains(const Math::Vector4<float>& v) const;
  Math::Vector3<float> size() const;
  Math::Vector4<float> center() const;
  const BBox& transform(const Math::Matrix4<float>& transform);

  const Math::Vector4<float>& lb() const { return _lb; }
  const Math::Vector4<float>& ub() const { return _ub; }

  friend std::ostream& operator<<(std::ostream& s, const BBox& b);

private:
  Math::Vector4<float> _lb;
  Math::Vector4<float> _ub;
};

std::ostream& operator<<(std::ostream& s, const BBox& b);

} /* namespace CSG */

#endif /* __BOX__ */
