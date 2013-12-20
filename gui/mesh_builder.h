#ifndef __MESH_BUILDER__
#define __MESH_BUILDER__

#include <list>
#include <map>
#include <tuple>
#include <utility>

#include "math/vector4.h"

namespace GUI
{

class MeshBuilder
{
public:
  typedef std::tuple<std::pair<float*, size_t>,
                     std::pair<short*, size_t>,
                     std::pair<short*, size_t> > Buffs;

  MeshBuilder (float threshold = 0.001);
  ~MeshBuilder ();

  void add_triangle (const Math::Vector4& pt1,
                     const Math::Vector4& pt2,
                     const Math::Vector4& pt3);
  Buffs finalize () const;

private:
  struct MeshNode
  {
    Math::Vector4 pt;
    short index;

    MeshNode (const Math::Vector4& v) : pt (v) { }
  };

  float _threshold;
  std::map<float, std::list<MeshNode*> > _vertex_map;
  std::list<MeshNode*> _wire_list;
  std::list<MeshNode*> _filled_list;

  MeshNode* add_vertex (const Math::Vector4& pt);
  float quantize (const Math::Vector4& pt);
};

} /* namespace GUI */

#endif /* __MESH_BUILDER__ */
