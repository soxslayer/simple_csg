#ifndef __VOXEL__
#define __VOXEL__

#include <ostream>

#include "bbox.h"
#include "math/vector3.h"

namespace CSG
{

class Voxel
{
public:
  Math::Vector3 center;
  double size;

  Voxel ();
  Voxel (const Math::Vector3& center, double size);
  Voxel (const BBox& b);
};

std::ostream& operator<< (std::ostream& s, const Voxel& vox);

} /* namespace CSG */

#endif /* __VOXEL__ */
