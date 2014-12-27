#include <tuple>

#include "surface_mesh.h"
#include "mesh_builder.h"
#include "tango_palette.h"
#include "math/util.h"

namespace GUI
{

int SurfaceMesh::MSEdgeTable[16] = {
  0x0, 0x9, 0x3, 0xa, 0x6, 0xf, 0x5, 0xc,
  0xc, 0x5, 0xf, 0x6, 0xa, 0x3, 0x9, 0x0
};

int SurfaceMesh::MSTriangleTable[16][13] = {
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 0, 5, 0, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 4, 7, 7, 4, 5, 7, 5, 6, 6, 5, 2, -1 },
  { 4, 1, 2, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 1, 7, 7, 1, 6, 6, 1, 2, -1, -1, -1, -1 },
  { 7, 6, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 4, 6, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 1, 5, 4, 5, 7, 7, 5, 6, 7, 6, 3, -1 },
  { 0, 1, 5, 0, 5, 6, 0, 6, 3, -1, -1, -1, -1 },
  { 7, 5, 2, 7, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 4, 3, 3, 4, 5, 3, 5, 2, -1, -1, -1, -1 },
  { 3, 7, 2, 2, 7, 4, 2, 4, 1, -1, -1, -1, -1 },
  { 0, 1, 2, 0, 2, 3, -1, -1, -1, -1, -1, -1, -1 }
};

SurfaceMesh::SurfaceMesh (const CSG::Surface& surface)
  : _surface (surface)
{
  MeshBuilder b;

  int cnt = 0;
  surface.traverse([&] (const CSG::Pixel& pix, bool filled)
    {
      float s2 = pix.size / 2;

      Math::Vector2 pts[8];
      float densities[4];
      int tbl_index = 0;

      pts[0] = pix.center;
      pts[0].x += s2;
      pts[0].y += s2;
      if (_surface.test_point (pts[0]))
        tbl_index |= 1;
      densities[0] = _surface.density (pts[0]);

      pts[1] = pix.center;
      pts[1].x += s2;
      pts[1].y -= s2;
      if (_surface.test_point (pts[1]))
        tbl_index |= 2;
      densities[1] = _surface.density (pts[1]);

      pts[2] = pix.center;
      pts[2].x -= s2;
      pts[2].y -= s2;
      if (_surface.test_point (pts[2]))
        tbl_index |= 4;
      densities[2] = _surface.density (pts[2]);

      pts[3] = pix.center;
      pts[3].x -= s2;
      pts[3].y += s2;
      if (_surface.test_point (pts[3]))
        tbl_index |= 8;
      densities[3] = _surface.density (pts[3]);

      if (MSEdgeTable[tbl_index] & 1) {
        pts[4] = Math::Vector2 (
          Math::f_interpolate (0, densities[0], pts[0].x,
                               densities[1], pts[1].x),
          Math::f_interpolate (0, densities[0], pts[0].y,
                               densities[1], pts[1].y));
      }
      if (MSEdgeTable[tbl_index] & 2) {
        pts[5] = Math::Vector2 (
          Math::f_interpolate (0, densities[1], pts[1].x,
                               densities[2], pts[2].x),
         Math::f_interpolate (0, densities[1], pts[1].y,
                               densities[2], pts[2].y));
      }
      if (MSEdgeTable[tbl_index] & 4) {
        pts[6] = Math::Vector2 (
          Math::f_interpolate (0, densities[2], pts[2].x,
                               densities[3], pts[3].x),
          Math::f_interpolate (0, densities[2], pts[2].y,
                               densities[3], pts[3].y));
      }
      if (MSEdgeTable[tbl_index] & 8) {
        pts[7] = Math::Vector2 (
          Math::f_interpolate (0, densities[3], pts[3].x,
                               densities[0], pts[0].x),
          Math::f_interpolate (0, densities[3], pts[3].y,
                               densities[0], pts[0].y));
      }

      for (int i = 0; MSTriangleTable[tbl_index][i] != -1; i += 3) {
        b.add_triangle (Math::Vector3 (
                          pts[MSTriangleTable[tbl_index][i]].x,
                          pts[MSTriangleTable[tbl_index][i]].y,
                          0),
                        Math::Vector3 (
                          pts[MSTriangleTable[tbl_index][i + 1]].x,
                          pts[MSTriangleTable[tbl_index][i + 1]].y,
                          0),
                        Math::Vector3 (
                          pts[MSTriangleTable[tbl_index][i + 2]].x,
                          pts[MSTriangleTable[tbl_index][i + 2]].y,
                          0));
        }

      return true;
    }, .1);

  MeshBuilder::Buffs buffs = b.finalize ();
  _v.alloc (std::get<0> (buffs).first, std::get<0> (buffs).second);
  _w.alloc (std::get<1> (buffs).first, std::get<1> (buffs).second);
  _f.alloc (std::get<2> (buffs).first, std::get<2> (buffs).second);

  const CSG::BRect& br = _surface.bounds ();
  MeshBuilder bb;

  bb.add_line (Math::Vector3 (br.lb.x, br.lb.y, 0),
               Math::Vector3 (br.lb.x, br.ub.y, 0));
  bb.add_line (Math::Vector3 (br.lb.x, br.ub.y, 0),
               Math::Vector3 (br.ub.x, br.ub.y, 0));
  bb.add_line (Math::Vector3 (br.ub.x, br.ub.y, 0),
               Math::Vector3 (br.ub.x, br.lb.y, 0));
  bb.add_line (Math::Vector3 (br.ub.x, br.lb.y, 0),
               Math::Vector3 (br.lb.x, br.lb.y, 0));

  MeshBuilder::Buffs bb_buffs = bb.finalize ();
  _bbv.alloc (std::get<0> (bb_buffs).first, std::get<0> (bb_buffs).second);
  _bbe.alloc (std::get<1> (bb_buffs).first, std::get<1> (bb_buffs).second);
}

void SurfaceMesh::draw (const Params& params)
{
  Drawable::draw (params);

  GUI::GPUPipeline& gpu = GUI::GPUPipeline::instance ();

  if (params.bounding_boxes) {
    gpu.bind_shader_input (_bbv, "vertex");
    gpu.bind_shader_input (Tango::MediumBlue.data (), "color");
    _bbe.bind ();

    gpu.draw_elements (GL_LINES, _bbe.get_count (), 0);
  }

  gpu.bind_shader_input (_v, "vertex");
  gpu.bind_shader_input (Tango::White.data (), "color");

  if (params.wireframe) {
    _w.bind ();
    gpu.draw_elements (GL_LINES, _w.get_count (), 0);
  }
  else {
    _f.bind ();
    gpu.draw_elements (GL_TRIANGLES, _f.get_count (), 0);
  }
}

} /* namespace GUI */
