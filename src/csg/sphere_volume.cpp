#include "sphere_volume.h"

namespace CSG
{

SphereVolume::SphereVolume(float radius)
  : Volume(BBox(radius * 2 + 0.01, radius * 2 + 0.01, radius * 2 + 0.01)),
    _radius(radius)
{
}

float SphereVolume::density(const Math::Vector4<float>& pt) const
{
  return _radius - pt.pt_length();
}

Math::Vector4<float> SphereVolume::normal(const Math::Vector4<float>& pt) const
{
  Math::Vector4<float> normal = pt;
  normal.normalize();

  return normal;
}

bool SphereVolume::intersects(const BBox& bbox) const
{
  BBox::CornerList corners = bbox.corners();

  for (int i = 0; i < corners.size(); ++i) {
    if (this->bbox().contains(corners[i]))
      return true;
  }

  return false;
}

} /* namespace CSG */
