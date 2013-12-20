#ifndef __BOX__
#define __BOX__

#include <ostream>

#include "math/vector3.h"

namespace CSG
{

class BBox
{
public:
  const Math::Vector3& lb;
  const Math::Vector3& ub;

  BBox ();
  BBox (const Math::Vector3& center, float dim_x,
        float dim_y, float dim_z);
  BBox (float dim_x, float dim_y, float dim_z);
  BBox (const Math::Vector3& v1, const Math::Vector3& v2);
  BBox (const BBox& b);
  const BBox& operator= (const BBox& b);

  bool intersects (const Math::Vector3& v) const;
  Math::Vector3 size () const;
  Math::Vector3 center () const;

  static BBox intersection (const BBox& b1, const BBox& b2);
  static BBox difference (const BBox& b1, const BBox& b2);
  static BBox combination (const BBox& b1, const BBox& b2);

private:
  Math::Vector3 _lb;
  Math::Vector3 _ub;
};

std::ostream& operator<< (std::ostream& s, const BBox& bb);

} /* namespace CSG */

#endif /* __BOX__ */
