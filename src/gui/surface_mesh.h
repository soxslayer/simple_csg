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
  SurfaceMesh (const CSG::Surface& surface);

  virtual void draw (const Params& params);

private:
  const CSG::Surface& _surface;
  float _bb_buff[8];
  GUI::GPUPipeline::VertexBuffer _bbv;
  GUI::GPUPipeline::ElementBuffer _bbe;
  GUI::GPUPipeline::VertexBuffer _v;
  GUI::GPUPipeline::ElementBuffer _w;
  GUI::GPUPipeline::ElementBuffer _f;

  static int MSEdgeTable[16];
  static int MSTriangleTable[16][13];
};

} /* namespace GUI */

#endif /* __MESH_SURFACE__ */
