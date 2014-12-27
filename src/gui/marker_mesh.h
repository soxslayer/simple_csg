#ifndef __MARKER_MESH__
#define __MARKER_MESH__

#include "drawable.h"
#include "mesh.h"
#include "math/vector.h"

namespace GUI
{

class MarkerMesh : public Drawable
{
public:
  MarkerMesh(float x, float y, float z, float size = 1);
  MarkerMesh(const Math::Vector3<float>& pt, float size = 1);
  MarkerMesh(const Math::Vector4<float>& pt, float size = 1);

  virtual void set_color(float r, float g, float b, float a);
  virtual void draw(const Params& params);

private:
  Mesh _mesh;
};

} /* namespace GUI */

#endif /* __MARKER_MESH__ */
