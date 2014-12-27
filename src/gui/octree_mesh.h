#ifndef __OCTREE_MESH__
#define __OCTREE_MESH__

#include "drawable.h"
#include "bbox_mesh.h"
#include "csg/bbox.h"
#include "csg/octree.h"

namespace GUI
{

template<typename T>
class OctreeMesh : public Drawable
{
public:
  OctreeMesh(CSG::Octree<T>& octree)
  {
    auto node = octree.nbegin();
    auto node_end = octree.nend();

    for (; node != node_end; ++node) {
      CSG::BBox bbox(node->center(), node->size());
      _meshes.emplace_back(bbox);
    }
  }

  using Drawable::set_color;
  virtual void set_color(float r, float g, float b, float a)
  {
    for (auto m = _meshes.begin(); m != _meshes.end(); ++m)
      m->set_color(r, g, b, a);
  }

  virtual void draw(const Params& params)
  {
    for (auto m = _meshes.begin(); m != _meshes.end(); ++m)
      m->draw(params);
  }

private:
  std::list<BBoxMesh> _meshes;
};

} /* namespace GUI */

#endif /* __OCTREE_MESH__ */
