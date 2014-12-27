#ifndef __MESH_VISUALIZATION__MESH_
#define __MESH_VISUALIZATION__MESH_

#include <vector>

#include "visualization.h"
#include "voxel.h"
#include "math/vector3.h"

namespace CSG
{

class Volume;

class MeshVisualization : public Visualization
{
public:
  MeshVisualization (const Volume& v);

  void color (double r, double g, double b);
  void regenerate () { _vol_hash += 1; }
  void stipple (bool s = true) { _stipple = s; }

  virtual void draw ();
  bool __dbg;

private:
  const Volume& _vol;
  int _vol_hash;
  std::vector<Math::Vector3> _mesh;
  double _r;
  double _g;
  double _b;
  bool _stipple;

  static int _mc_edge_table[256];
  static int _mc_triangle_table[256][15];

  void generate_mesh ();

  static Math::Vector3 edge_intersection (const Math::Vector3& pt1,
                                          const Math::Vector3& pt2,
                                          double d1, double d2);
};

} /* namespace CSG */

#endif /* __MESH_VISUALIZATION__MESH_ */
