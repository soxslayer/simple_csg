#ifndef __VOLUME_MESH__
#define __VOLUME_MESH__

#include <list>
#include <utility>

#include "bbox_mesh.h"
#include "drawable.h"
#include "mesh.h"
#include "mesh_builder.h"
#include "octree_mesh.h"
#include "csg/bbox.h"
#include "csg/octree.h"

namespace CSG
{
  class Volume;
} /* namespace CSG */

namespace GUI
{

class VolumeMesh : public Drawable
{
private:
  typedef std::pair<Math::Vector4<float>, int> FeaturePair;

public:
  VolumeMesh(const CSG::Volume& volume);

  virtual void set_color(float r, float g, float b, float a);
  virtual void draw(const Params& params);

  Mesh _mesh;
private:
  BBoxMesh _bbox_mesh;

  Math::Vector4<float> edge_intersection(const Math::Vector4<float>& pt1,
                                         const Math::Vector4<float>& pt2,
                                         float d1, float d2);
  FeaturePair get_feature_point(const CSG::Volume& volume,
                                const CSG::BBox::CornerList& corners, bool dbg=false);
  int check_edge(int ivec, int c1, int c2);
  void add_quad(MeshBuilder& builder, CSG::Octree<FeaturePair>::Iterator& vox,
                int x1, int y1, int z1, int x2, int y2, int z2, int dir);
  CSG::Octree<FeaturePair>::Iterator get_neighbor(
      CSG::Octree<FeaturePair>::Iterator& vox, int x, int y, int z);
  float get_neighbor_distance(CSG::Octree<FeaturePair>::Iterator& pt,
                              CSG::Octree<FeaturePair>::Iterator& neighbor,
                              int x, int y, int z);
};

} /* namespace GUI */

#endif /* __VOLUME_MESH__ */
