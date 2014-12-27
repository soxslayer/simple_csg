#include "oct_traverse.h"
#include "bbox.h"
#include "volume.h"

namespace CSG
{

OctTraverse::OctTraverse (const Volume& vol)
  : _vol (vol)
{
}

void OctTraverse::traverse (const Callback& callback) const
{
  Math::Vector3<float> bbox_dim = _vol.bbox().size();
  float s = bbox_dim[0];

  for (int i = 1; i < 3; ++i) {
    if (bbox_dim[i] > s)
      s = bbox_dim[i];
  }

  // Make a cube bbox
  traverse_impl (callback, BBox (s, s, s));
}

void OctTraverse::traverse_impl (const Callback& callback,
                                 const BBox& bbox) const
{
  if (!callback (bbox))
    return;

  float s2 = bbox.size()[0] / 2;
  float s4 = s2 / 2;

  for (int i = 0; i < 8; ++i) {
    Math::Vector4<float> translate;

    if (i & 1)
      translate[0] = -s4;
    else
      translate[0] = s4;

    if (i & 2)
      translate[1] = -s4;
    else
      translate[1] = s4;

    if (i & 4)
      translate[2] = -s4;
    else
      translate[2] = s4;

    //BBox sub_bbox (bbox, transform);
    BBox sub_bbox (bbox.center() + translate, s2);
    traverse_impl (callback, sub_bbox);
  }
}

} /* namespace CSG */
