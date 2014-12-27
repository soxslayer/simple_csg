#ifndef __BBOX_MESH__
#define __BBOX_MESH__

#include "drawable.h"
#include "mesh.h"
#include "math/vector.h"

namespace CSG
{
  class BBox;
} /* namespace CSG */

namespace GUI
{

class BBoxMesh : public Drawable
{
public:
  BBoxMesh (const CSG::BBox& bbox);

  using Drawable::set_color;
  virtual void set_color (float r, float g, float b, float a);
  virtual void draw (const Params& params);

private:
  Mesh _mesh;
};

} /* namespace GUI */

#endif /* __BBOX_MESH__ */
