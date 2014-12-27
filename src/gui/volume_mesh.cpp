#include "volume_mesh.h"

#include "gpu_pipeline.h"
#include "csg/oct_traverse.h"
#include "csg/volume.h"
#include "gui/marker_mesh.h"
#include "gui/tango_palette.h"
#include "math/util.h"

namespace GUI
{

VolumeMesh::VolumeMesh(const CSG::Volume& volume)
  : _bbox_mesh(volume.bbox())
{
  CSG::OctTraverse oct(volume);
  MeshBuilder builder;
  Math::Vector3<float> v_size = volume.bbox().size();
  float s = v_size[0];

  if (v_size[1] > s)
    s = v_size[1];
  if (v_size[2] > s)
    s = v_size[2];

  float resolution = s / 5;
  CSG::Octree<FeaturePair> octree(s, 1);

  oct.traverse(
    [&](const CSG::BBox& vox)
    {
      CSG::BBox::CornerList corners = vox.corners();
      auto corner = corners.begin();
      auto corner_end = corners.end();
      bool filled = true;
      bool empty = true;
      Math::Vector3<float> d = vox.size();

      for (; corner != corner_end; ++corner) {
        if (volume.test_point(*corner))
          empty = false;
        else
          filled = false;
      }

      if (filled || (empty && !volume.intersects(vox)))
        return false;

      if (d[0] <= resolution || d[1] <= resolution || d[2] <= resolution) {
        FeaturePair p = get_feature_point(volume, corners);
        if (p.second != 0 && p.second != 0b11111111)
          octree.add_item(p, p.first);

        return false;
      }

      return true;
    }
  );

  auto vox = octree.begin();
  auto vox_end = octree.end();

  for (; vox != vox_end; ++vox) {
    int c = check_edge(vox->second, 0, 1);
    if (c)
      add_quad(builder, vox, 0, -1, 0, 0, 0, -1, c);

    c = check_edge(vox->second, 2, 0);
    if (c)
      add_quad(builder, vox, -1, 0, 0, 0, 0, -1, c);

    c = check_edge(vox->second, 0, 4);
    if (c)
      add_quad(builder, vox, -1, 0, 0, 0, -1, 0, c);
  }

  _mesh = builder.finalize();
  _bbox_mesh.set_color(Tango::Blue);
}

void VolumeMesh::set_color(float r, float g, float b, float a)
{
  _mesh.set_color(r, g, b, a);
}

void VolumeMesh::draw(const Params& params)
{
  _mesh.draw(params);

  if (params.bounding_boxes)
    _bbox_mesh.draw(params);
}

Math::Vector4<float> VolumeMesh::edge_intersection(
    const Math::Vector4<float>& pt1,
    const Math::Vector4<float>& pt2,
    float d1, float d2)
{
  Math::Vector4<float> r = { pt1[0], pt1[1], pt1[2], 1 };
  float m = d1 / (d1 - d2);

  r[0] += m * (pt2[0] - pt1[0]);
  r[1] += m * (pt2[1] - pt1[1]);
  r[2] += m * (pt2[2] - pt1[2]);

  return r;
}

VolumeMesh::FeaturePair VolumeMesh::get_feature_point(
    const CSG::Volume& volume, const CSG::BBox::CornerList& corners,
    bool dbg)
{
  float densities[8];
  int ivec = 0;

  for (int i = 0; i < 8; ++i) {
    densities[i] = volume.density(corners[i]);
    if (densities[i] >= 0)
      ivec |= 1 << i;
  }

  if (!ivec || ivec == 0b11111111)
    return std::make_pair(Math::Vector4<float>(), 0);

  Math::Vector4<float> edges[12];
  Math::Vector4<float> norms[12];
  Math::Vector4<float> centroid;
  int n_edges = 0;

  if (check_edge(ivec, 0, 1)) {
    edges[0] = edge_intersection(corners[0], corners[1],
                                 densities[0], densities[1]);
    norms[0] = volume.normal(edges[0]);
    centroid += edges[0];
    ++n_edges;
  }
  if (check_edge(ivec, 1, 3)) {
    edges[1] = edge_intersection(corners[1], corners[3],
                                 densities[1], densities[3]);
    centroid += edges[1];
    ++n_edges;
  }
  if (check_edge(ivec, 3, 2)) {
    edges[2] = edge_intersection(corners[3], corners[2],
                                 densities[3], densities[2]);
    norms[2] = volume.normal(edges[2]);
    centroid += edges[2];
    ++n_edges;
  }
  if (check_edge(ivec, 2, 0)) {
    edges[3] = edge_intersection(corners[2], corners[0],
                                 densities[2], densities[0]);
    norms[3] = volume.normal(edges[3]);
    centroid += edges[3];
    ++n_edges;
  }
  if (check_edge(ivec, 4, 5)) {
    edges[4] = edge_intersection(corners[4], corners[5],
                                 densities[4], densities[5]);
    norms[4] = volume.normal(edges[4]);
    centroid += edges[4];
    ++n_edges;
  }
  if (check_edge(ivec, 5, 7)) {
    edges[5] = edge_intersection(corners[5], corners[7],
                                 densities[5], densities[7]);
    norms[5] = volume.normal(edges[5]);
    centroid += edges[5];
    ++n_edges;
  }
  if (check_edge(ivec, 7, 6)) {
    edges[6] = edge_intersection(corners[7], corners[6],
                                 densities[7], densities[6]);
    norms[6] = volume.normal(edges[6]);
    centroid += edges[6];
    ++n_edges;
  }
  if (check_edge(ivec, 6, 4)) {
    edges[7] = edge_intersection(corners[6], corners[4],
                                 densities[6], densities[4]);
    norms[7] = volume.normal(edges[7]);
    centroid += edges[7];
    ++n_edges;
  }
  if (check_edge(ivec, 0, 4)) {
    edges[8] = edge_intersection(corners[0], corners[4],
                                 densities[0], densities[4]);
    norms[8] = volume.normal(edges[8]);
    centroid += edges[8];
    ++n_edges;
  }
  if (check_edge(ivec, 1, 5)) {
    edges[9] = edge_intersection(corners[1], corners[5],
                                 densities[1], densities[5]);
    norms[9] = volume.normal(edges[9]);
    centroid += edges[9];
    ++n_edges;
  }
  if (check_edge(ivec, 2, 6)) {
    edges[10] = edge_intersection(corners[2], corners[6],
                                  densities[2], densities[6]);
    norms[10] = volume.normal(edges[10]);
    centroid += edges[10];
    ++n_edges;
  }
  if (check_edge(ivec, 3, 7)) {
    edges[11] = edge_intersection(corners[3], corners[7],
                                  densities[3], densities[7]);
    norms[11] = volume.normal(edges[11]);
    centroid += edges[11];
    ++n_edges;
  }

  centroid /= n_edges;

  // particle-based feature approximation goes here

  return std::make_pair(centroid, ivec);
}

int VolumeMesh::check_edge(int ivec, int c1, int c2)
{
  if (((ivec >> c1) & 1) && !((ivec >> c2) & 1))
    return 1;
  else if (!((ivec >> c1) & 1) && ((ivec >> c2) & 1))
    return -1;

  return 0;
}

void VolumeMesh::add_quad(MeshBuilder& builder,
                          CSG::Octree<FeaturePair>::Iterator& vox,
                          int x1, int y1, int z1, int x2, int y2, int z2,
                          int dir)
{
  auto pt1 = get_neighbor(vox, x1, y1, z1);
  auto pt2 = get_neighbor(vox, x2, y2, z2);

  if (!pt1.get_node() || ! pt2.get_node())
    return;

  auto pt3 = get_neighbor(pt2, x1, y1, z1);

  if (!pt3.get_node())
    return;

  if (dir > 0)
    builder.add_quad(vox->first, pt1->first, pt3->first, pt2->first);
  else
    builder.add_quad(pt2->first, pt3->first, pt1->first, vox->first);
}

CSG::Octree<VolumeMesh::FeaturePair>::Iterator VolumeMesh::get_neighbor(
      CSG::Octree<FeaturePair>::Iterator& vox, int x, int y, int z)
{
  auto pt = vox.get_node()->get_neighbor(x, y, z);
  auto pt_end = vox.get_node()->end();

  CSG::Octree<FeaturePair>::Iterator ret = pt;
  ++pt;
  float q = get_neighbor_distance(vox, ret, x, y, z);

  /* When a larger voxel is bordered by smaller voxels we need to choose the
   * the correct neighbor because there may be multiple ones. We do this by
   * minimizing the distance along the dimensions we're not looking for the
   * neighbor in. I.e. if we look for the neighbor along the y axis minimize
   * the length along the x and z coordinates to find the correct one.
   */
  for (; pt != pt_end; ++pt) {
    if (ret != pt_end) {
      float nq = get_neighbor_distance(vox, pt, x, y, z);
      if (nq < q) {
        q = nq;
        ret = pt;
      }
    }
  }

  return ret;
}

float VolumeMesh::get_neighbor_distance(CSG::Octree<FeaturePair>::Iterator& pt,
    CSG::Octree<FeaturePair>::Iterator& neighbor, int x, int y, int z)
{
  if (!pt.get_node())
    return -1;
  if (!neighbor.get_node())
    return -1;

  float sum = 0;

  if (!x) {
    float d = pt->first[0] - neighbor->first[0];
    sum += d * d;
  }
  if (!y) {
    float d = pt->first[1] - neighbor->first[1];
    sum += d * d;
  }
  if (!z) {
    float d = pt->first[2] - neighbor->first[2];
    sum += d * d;
  }

  return sqrt(sum);
}

} /* namespace GUI */
