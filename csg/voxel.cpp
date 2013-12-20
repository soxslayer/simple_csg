#include "voxel.h"
#include "math/util.h"

namespace CSG
{

Voxel::Voxel ()
{
}

Voxel::Voxel (const Math::Vector3& center, double size)
  : center (center), size (size)
{
}

Voxel::Voxel (const BBox& b)
  : center (b.center ())
{
  Math::Vector3 bsize = b.size ();

  size = bsize.x;
  if (size < bsize.y) size = bsize.y;
  if (size < bsize.z) size = bsize.z;
}

std::ostream& operator<< (std::ostream& s, const Voxel& vox)
{
  s << vox.center << ' ' << vox.size;

  return s;
}

} /* namespace CSG */
