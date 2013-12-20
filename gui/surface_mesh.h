#ifndef __SURFACE_MESH__
#define __SURFACE_MESH__

#include "gpu_pipeline.h"
#include "drawable.h"
#include "csg/surface.h"

namespace GUI
{

class SurfaceMesh : public Drawable
{
public:
  //SurfaceMesh (const CSG::Surface& surface);
  SurfaceMesh ();

  virtual void draw ();

private:
  //const CSG::Surface& _surface;
  GUI::GPUPipeline::VertexBuffer _v;
  GUI::GPUPipeline::ElementBuffer _w;
  GUI::GPUPipeline::ElementBuffer _f;
};

} /* namespace GUI */

#endif /* __MESH_SURFACE__ */
