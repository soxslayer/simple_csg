#include "util/debug.h"
#include "marker_mesh.h"

#include "gui/mesh_builder.h"

namespace GUI
{

MarkerMesh::MarkerMesh(float x, float y, float z, float size)
{
  float sqrt3 = 1.73205080757;
  float sqrt2 = 1.41421356237;
  float s2 = size / 2;
  float c2 = s2 / sqrt2;
  float c3 = s2 / sqrt3;

  MeshBuilder builder;
  builder.add_line(Math::Vector3<float> ({ x + s2, y, z }),
                   Math::Vector3<float> ({ x - s2, y, z }));
  builder.add_line(Math::Vector3<float> ({ x, y + s2, z }),
                   Math::Vector3<float> ({ x, y - s2, z }));
  builder.add_line(Math::Vector3<float> ({ x, y, z + s2 }),
                   Math::Vector3<float> ({ x, y, z - s2 }));
  builder.add_line(Math::Vector3<float> ({ x + c2, y + c2, z }),
                   Math::Vector3<float> ({ x - c2, y - c2, z }));
  builder.add_line(Math::Vector3<float> ({ x + c2, y - c2, z }),
                   Math::Vector3<float> ({ x - c2, y + c2, z }));
  builder.add_line(Math::Vector3<float> ({ x, y + c2, z + c2}),
                   Math::Vector3<float> ({ x, y - c2, z - c2}));
  builder.add_line(Math::Vector3<float> ({ x, y + c2, z - c2}),
                   Math::Vector3<float> ({ x, y - c2, z + c2}));
  builder.add_line(Math::Vector3<float> ({ x + c2, y, z + c2 }),
                   Math::Vector3<float> ({ x - c2, y, z - c2 }));
  builder.add_line(Math::Vector3<float> ({ x - c2, y, z + c2 }),
                   Math::Vector3<float> ({ x + c2, y, z - c2 }));

  _mesh = builder.finalize ();
}

MarkerMesh::MarkerMesh(const Math::Vector3<float>& pt, float size)
  : MarkerMesh(pt[0], pt[1], pt[2], size)
{
}

MarkerMesh::MarkerMesh(const Math::Vector4<float>& pt, float size)
  : MarkerMesh(pt[0], pt[1], pt[2], size)
{
}

void MarkerMesh::set_color(float r, float g, float b, float a)
{
  _mesh.set_color(r, g, b, a);
}

void MarkerMesh::draw(const Params& params)
{
  Params my_params = params;
  my_params.wireframe = true;
  _mesh.draw(my_params);
}

} /* namespace GUI */
