#ifndef __MESH_BUILDER__
#define __MESH_BUILDER__

#include <list>
#include <map>
#include <tuple>
#include <utility>

#include "mesh.h"
#include "math/vector.h"

namespace GUI
{

class MeshBuilder
{
public:
  MeshBuilder(float threshold = 0.001);
  ~MeshBuilder();

  void queue_point(const Math::Vector3<float>& pt);
  void queue_clear();
  void add_point(const Math::Vector3<float>& pt);
  void add_line(const Math::Vector3<float>& pt1,
                const Math::Vector3<float>& pt2);
  void add_lines();
  void add_triangle(const Math::Vector3<float>& pt1,
                    const Math::Vector3<float>& pt2,
                    const Math::Vector3<float>& pt3);
  void add_triangles();
  void add_quad(const Math::Vector3<float>& pt1,
                const Math::Vector3<float>& pt2,
                const Math::Vector3<float>& pt3,
                const Math::Vector3<float>& pt4);
  void add_quad(const Math::Vector4<float>& pt1,
                const Math::Vector4<float>& pt2,
                const Math::Vector4<float>& pt3,
                const Math::Vector4<float>& pt4);
  void add_quads();
  bool contains_point(const Math::Vector3<float>& pt)
  { get_vertex(pt) != nullptr; }
  Mesh finalize() const;

private:
  struct MeshNode
  {
    Math::Vector3<float> pt;
    short index;

    MeshNode(const Math::Vector3<float>& v) : pt(v) { }
  };

  float _threshold;
  std::map<float, std::list<MeshNode*>> _vertex_map;
  std::list<MeshNode*> _queue;
  std::list<MeshNode*> _wire_list;
  std::list<MeshNode*> _filled_list;

  void add_point(MeshNode* n);
  void add_line(MeshNode* n1, MeshNode* n2);
  void add_triangle(MeshNode* n1, MeshNode* n2, MeshNode* n3);
  void add_quad(MeshNode* n1, MeshNode* n2, MeshNode* n3, MeshNode* n4);

  MeshNode* get_vertex(const Math::Vector3<float>& pt);
  MeshNode* add_vertex(const Math::Vector3<float>& pt);
  float quantize(const Math::Vector3<float>& pt);
};

} /* namespace GUI */

#endif /* __MESH_BUILDER__ */
