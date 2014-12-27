#ifndef __SPHERE_VOLUME__
#define __SPHERE_VOLUME__

#include "volume.h"

namespace CSG
{

class SphereVolume : public Volume
{
public:
  SphereVolume(float radius);

  virtual float density(const Math::Vector4<float>& pt) const;
  virtual Math::Vector4<float> normal(const Math::Vector4<float>& pt) const;
  virtual bool intersects(const BBox& bbox) const;

private:
  float _radius;
};

} /* namespace CSG */

#endif /* __SPHERE_VOLUME__ */
