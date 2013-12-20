#include "surface_mesh.h"
#include "mesh_builder.h"

using namespace std;

namespace GUI
{

//SurfaceMesh::SurfaceMesh (const CSG::Surface& surface)
  //: _surface (surface)
SurfaceMesh::SurfaceMesh ()
{
  MeshBuilder b;

  b.add_triangle (Math::Vector4 (1.0, 1.0, 0.0, 1.0),
                  Math::Vector4 (1.0, -1.0, 0.0, 1.0),
                  Math::Vector4 (-1.0, 1.0, 0.0, 1.0));
  b.add_triangle (Math::Vector4 (1.0, -1.0, 0.0, 1.0),
                  Math::Vector4 (-1.0, 1.0, 0.0, 1.0),
                  Math::Vector4 (-1.0, -1.0, 0.0, 1.0));

  MeshBuilder::Buffs buffs = b.finalize ();
  _v.alloc (get<0> (buffs).first, get<0> (buffs).second);
  _w.alloc (get<1> (buffs).first, get<1> (buffs).second);
  _f.alloc (get<2> (buffs).first, get<2> (buffs).second);
}

void SurfaceMesh::draw ()
{
  Drawable::draw ();

  GUI::GPUPipeline& gpu = GUI::GPUPipeline::instance ();

  gpu.bind_shader_input (_v, "vertex");
  _w.bind ();
  //_f.bind ();

  gpu.draw_elements (GL_LINES, _w.get_count (), 0);
  //gpu.draw_elements (GL_TRIANGLES, _f.get_count (), 0);
}

} /* namespace GUI */
