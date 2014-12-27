#include <vector>
#include <cmath>
#include <cstring>

#include "mesh_builder.h"
#include "math/util.h"

using namespace std;

namespace GUI
{

MeshBuilder::MeshBuilder(float threshold)
  : _threshold(threshold)
{
}

MeshBuilder::~MeshBuilder()
{
  auto mi = _vertex_map.begin();
  auto me = _vertex_map.end();

  for (; mi != me; ++mi) {
    auto li = mi->second.begin();
    auto le = mi->second.end();

    for (; li != le; ++li)
      delete *li;
  }
}

void MeshBuilder::queue_point(const Math::Vector3<float>& pt)
{
  _queue.push_back(add_vertex(pt));
}

void MeshBuilder::queue_clear()
{
  _queue.clear();
}

void MeshBuilder::add_point(const Math::Vector3<float>& pt)
{
  add_point(add_vertex(pt));
}

void MeshBuilder::add_line(const Math::Vector3<float>& pt1,
                           const Math::Vector3<float>& pt2)
{
  add_line(add_vertex(pt1), add_vertex(pt2));
}

void MeshBuilder::add_lines()
{
  while (_queue.size() >= 2) {
    MeshNode* n1 = _queue.front(); _queue.pop_front();
    MeshNode* n2 = _queue.front(); _queue.pop_front();

    add_line(n1, n2);
  }
}

void MeshBuilder::add_triangle(const Math::Vector3<float>& pt1,
                               const Math::Vector3<float>& pt2,
                               const Math::Vector3<float>& pt3)
{
  add_triangle(add_vertex(pt1), add_vertex(pt2), add_vertex(pt3));
}

void MeshBuilder::add_triangles()
{
  while (_queue.size() >= 3) {
    MeshNode* n1 = _queue.front(); _queue.pop_front();
    MeshNode* n2 = _queue.front(); _queue.pop_front();
    MeshNode* n3 = _queue.front(); _queue.pop_front();

    add_triangle(n1, n2, n3);
  }
}

void MeshBuilder::add_quad(const Math::Vector3<float>& pt1,
                           const Math::Vector3<float>& pt2,
                           const Math::Vector3<float>& pt3,
                           const Math::Vector3<float>& pt4)
{
  add_triangle(pt1, pt2, pt3);
  add_triangle(pt3, pt4, pt1);
}

void MeshBuilder::add_quad(const Math::Vector4<float>& pt1,
                           const Math::Vector4<float>& pt2,
                           const Math::Vector4<float>& pt3,
                           const Math::Vector4<float>& pt4)
{
  add_quad(Math::Vector3<float>({ pt1[0], pt1[1], pt1[2] }),
           Math::Vector3<float>({ pt2[0], pt2[1], pt2[2] }),
           Math::Vector3<float>({ pt3[0], pt3[1], pt3[2] }),
           Math::Vector3<float>({ pt4[0], pt4[1], pt4[2] }));
}

void MeshBuilder::add_quads()
{
  while (_queue.size() >= 3) {
    MeshNode* n1 = _queue.front(); _queue.pop_front();
    MeshNode* n2 = _queue.front(); _queue.pop_front();
    MeshNode* n3 = _queue.front(); _queue.pop_front();
    MeshNode* n4 = _queue.front(); _queue.pop_front();

    add_quad(n1, n2, n3, n4);
  }
}

Mesh MeshBuilder::finalize() const
{
  vector<float> verticies;
  vector<short> wire_elements;
  vector<short> filled_elements;

  auto mi = _vertex_map.begin();
  auto me = _vertex_map.end();
  short element = 0;

  for (; mi != me; ++mi) {
    auto li = mi->second.begin();
    auto le = mi->second.end();

    for (; li != le; ++li) {
      verticies.push_back((*li)->pt[0]);
      verticies.push_back((*li)->pt[1]);
      verticies.push_back((*li)->pt[2]);
      verticies.push_back(1.0);

      (*li)->index = element++;
    }
  }

  auto li = _wire_list.begin();
  auto le = _wire_list.end();

  for (; li != le; ++li)
    wire_elements.push_back((*li)->index);

  li = _filled_list.begin();
  le = _filled_list.end();

  for (; li != le; ++li)
    filled_elements.push_back((*li)->index);

  return Mesh(verticies, wire_elements, filled_elements);
}

void MeshBuilder::add_point(MeshNode* n)
{
  _wire_list.push_back(n);
  _filled_list.push_back(n);
}

void MeshBuilder::add_line(MeshNode* n1, MeshNode* n2)
{
  _wire_list.push_back(n1);
  _wire_list.push_back(n2);

  _filled_list.push_back(n1);
  _filled_list.push_back(n2);
}

void MeshBuilder::add_triangle(MeshNode* n1, MeshNode* n2, MeshNode* n3)
{
  _wire_list.push_back(n1);
  _wire_list.push_back(n2);
  _wire_list.push_back(n2);
  _wire_list.push_back(n3);
  _wire_list.push_back(n3);
  _wire_list.push_back(n1);

  _filled_list.push_back(n1);
  _filled_list.push_back(n2);
  _filled_list.push_back(n3);
}

void MeshBuilder::add_quad(MeshNode* n1, MeshNode* n2,
                           MeshNode* n3, MeshNode* n4)
{
  add_triangle(n1, n2, n3);
  add_triangle(n3, n4, n1);
}

MeshBuilder::MeshNode* MeshBuilder::get_vertex(const Math::Vector3<float>& pt)
{
  float q = quantize(pt);

  auto lb = _vertex_map.lower_bound(q - _threshold);
  auto ub = _vertex_map.upper_bound(q + _threshold);

  for (; lb != ub; ++lb) {
    auto n = lb->second.begin();
    auto n_end = lb->second.end();

    for (; n != n_end; ++n) {
      float delta = (pt - (*n)->pt).length();

      if (Math::between(delta, -_threshold, _threshold))
        return *n;
    }
  }

  return nullptr;
}

MeshBuilder::MeshNode* MeshBuilder::add_vertex(const Math::Vector3<float>& pt)
{
  MeshNode* node = get_vertex(pt);

  if (node == nullptr) {
    float q = quantize(pt);
    node = new MeshNode(pt);
    _vertex_map[q].push_back(node);
  }

  return node;
}

float MeshBuilder::quantize(const Math::Vector3<float>& pt)
{
  float x = pt[0] + pt[1] + pt[2];

  return copysign(_threshold
                  * floorf(Math::abs(x) / _threshold + 0.5), x);
}

} /* namespace GUI */
