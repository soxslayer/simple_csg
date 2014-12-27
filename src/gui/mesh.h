#ifndef __MESH__
#define __MESH__

#include <vector>

#include "drawable.h"
#include "gpu_pipeline.h"

namespace GUI
{

class DebugMesh;

class Mesh : public Drawable
{
  friend class DebugMesh;

public:
  Mesh() { }
  Mesh(Mesh&& m);
  Mesh(const std::vector<float>& verticies,
       const std::vector<short>& wireframe_elements,
       const std::vector<short>& filled_elements);
  const Mesh& operator=(Mesh&& m);

  void draw(const Params& params, int n_elements, int offset);
  virtual void draw(const Params& params);

private:
  GPUPipeline::VertexBuffer _verticies;
  GPUPipeline::ElementBuffer _wireframe;
  GPUPipeline::ElementBuffer _filled;

  void draw_elements(GPUPipeline::ElementBuffer& elements,
                     int item_type, int item_size, int n_items, int offset);
};

} /* namespace GUI */

#endif /* __MESH__ */
