#include "volume.h"

namespace CSG
{

Volume::Volume()
{
}

Volume::Volume(const BBox& bbox)
  : _bbox(bbox)
{
}

Volume::~Volume()
{
}

const BBox& Volume::bbox() const
{
  return _bbox;
}

bool Volume::test_point(const Math::Vector4<float>& pt) const
{
  return density(pt) >= 0;
}

} /* namespace CSG */
