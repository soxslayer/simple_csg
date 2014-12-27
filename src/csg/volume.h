#ifndef __VOLUME__
#define __VOLUME__

#include "bbox.h"
#include "math/matrix.h"
#include "math/vector.h"

namespace CSG
{

class Volume
{
public:
  Volume();
  virtual ~Volume() = 0;

  const BBox& bbox() const;

  bool test_point(const Math::Vector4<float>& pt) const;
  virtual float density(const Math::Vector4<float>& pt) const = 0;
  virtual Math::Vector4<float> normal(const Math::Vector4<float>& pt) const = 0;
  /* Try and return the most accurate results. Must never return a false
     negative. */
  virtual bool intersects(const BBox& bbox) const = 0;

protected:
  Volume(const BBox& bbox);

private:
  BBox _bbox;
};

} /* namespace CSG */

#endif /* __VOLUME__ */
