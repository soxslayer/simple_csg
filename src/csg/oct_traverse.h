#ifndef __OCT_TRAVERSE__
#define __OCT_TRAVERSE__

#include "volume_traverse.h"

namespace CSG
{

class Volume;

class OctTraverse : public VolumeTraverse
{
public:
  OctTraverse (const Volume& vol);

  virtual void traverse (const Callback& callback) const;

private:
  const Volume& _vol;

  void traverse_impl (const Callback& callback, const BBox& bbox) const;
};

} /* namespace CSG */

#endif /* __OCT_TRAVERSE__ */
