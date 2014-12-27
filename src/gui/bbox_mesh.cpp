#include "bbox_mesh.h"

#include "mesh_builder.h"

#include "csg/bbox.h"
#include "math/vector.h"

namespace GUI
{

BBoxMesh::BBoxMesh (const CSG::BBox& bbox)
{
  Math::Vector4<float> center = bbox.center ();
  Math::Vector3<float> s2 = bbox.size () / 2;
  MeshBuilder builder;

  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] - s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] - s2[1],
                            center[2] - s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] + s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] + s2[1],
                            center[2] - s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] - s2[1],
                            center[2] + s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] - s2[1],
                            center[2] + s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] + s2[1],
                            center[2] + s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] + s2[1],
                            center[2] + s2[2] }));

  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] - s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] - s2[0], center[1] + s2[1],
                            center[2] - s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] - s2[1],
                            center[2] + s2[2] }),
    Math::Vector3<float> ({ center[0] - s2[0], center[1] + s2[1],
                            center[2] + s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] + s2[0], center[1] - s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] + s2[1],
                            center[2] - s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] + s2[0], center[1] - s2[1],
                            center[2] + s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] + s2[1],
                            center[2] + s2[2] }));

  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] - s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] - s2[0], center[1] - s2[1],
                            center[2] + s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] - s2[0], center[1] + s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] - s2[0], center[1] + s2[1],
                            center[2] + s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] + s2[0], center[1] - s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] - s2[1],
                            center[2] + s2[2] }));
  builder.add_line (
    Math::Vector3<float> ({ center[0] + s2[0], center[1] + s2[1],
                            center[2] - s2[2] }),
    Math::Vector3<float> ({ center[0] + s2[0], center[1] + s2[1],
                            center[2] + s2[2] }));

  _mesh = builder.finalize ();
}

void BBoxMesh::set_color (float r, float g, float b, float a)
{
  _mesh.set_color (r, g, b, a);
}

void BBoxMesh::draw (const Params& params)
{
  Params my_params = params;
  my_params.wireframe = true;

  Drawable::draw (params);
  _mesh.draw (my_params);
}

} /* namespace GUI */
