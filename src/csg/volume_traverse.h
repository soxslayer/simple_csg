#ifndef __VOLUME_TRAVERSE__
#define __VOLUME_TRAVERSE__

#include <functional>

namespace CSG
{

class BBox;

class VolumeTraverse
{
public:
  typedef std::function<bool(const BBox&)> Callback;

  virtual ~VolumeTraverse () = 0;

  virtual void traverse (const Callback& callback) const = 0;
};

} /* namespace CSG */

#endif /* __VOLUME_TRAVERSE__ */
