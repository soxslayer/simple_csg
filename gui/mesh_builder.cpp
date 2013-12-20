#include <vector>
#include <cmath>
#include <cstring>

#include "mesh_builder.h"
#include "math/util.h"

using namespace std;

namespace GUI
{

MeshBuilder::MeshBuilder (float threshold)
  : _threshold (threshold)
{
}

MeshBuilder::~MeshBuilder ()
{
  auto mi = _vertex_map.begin ();
  auto me = _vertex_map.end ();

  for (; mi != me; ++mi) {
    auto li = mi->second.begin ();
    auto le = mi->second.end ();

    for (; li != le; ++li)
      delete *li;
  }
}

void MeshBuilder::add_triangle (const Math::Vector4& pt1,
                                const Math::Vector4& pt2,
                                const Math::Vector4& pt3)
{
  MeshNode* n1 = add_vertex (pt1);
  MeshNode* n2 = add_vertex (pt2);
  MeshNode* n3 = add_vertex (pt3);

  _wire_list.push_back (n1);
  _wire_list.push_back (n2);
  _wire_list.push_back (n2);
  _wire_list.push_back (n3);
  _wire_list.push_back (n3);
  _wire_list.push_back (n1);

  _filled_list.push_back (n1);
  _filled_list.push_back (n2);
  _filled_list.push_back (n3);
}

MeshBuilder::Buffs MeshBuilder::finalize () const
{
  vector<float> verticies;
  vector<short> wire_elements;
  vector<short> filled_elements;

  auto mi = _vertex_map.begin ();
  auto me = _vertex_map.end ();
  short element = 0;

  for (; mi != me; ++mi) {
    auto li = mi->second.begin ();
    auto le = mi->second.end ();

    for (; li != le; ++li) {
      verticies.push_back ((*li)->pt.x);
      verticies.push_back ((*li)->pt.y);
      verticies.push_back ((*li)->pt.z);
      verticies.push_back ((*li)->pt.w);

      (*li)->index = element++;
    }
  }

  auto li = _wire_list.begin ();
  auto le = _wire_list.end ();

  for (; li != le; ++li)
    wire_elements.push_back ((*li)->index);

  li = _filled_list.begin ();
  le = _filled_list.end ();

  for (; li != le; ++li)
    filled_elements.push_back ((*li)->index);

  float* vertex_buff = new float[verticies.size ()];
  short* wire_buff = new short[wire_elements.size ()];
  short* filled_buff = new short[filled_elements.size ()];

  memcpy (vertex_buff, verticies.data (), verticies.size () * sizeof (float));
  memcpy (wire_buff, wire_elements.data (),
          wire_elements.size () * sizeof (short));
  memcpy (filled_buff, filled_elements.data (),
          filled_elements.size () * sizeof (short));

  return Buffs (pair<float*, size_t> (vertex_buff, verticies.size ()),
                pair<short*, size_t> (wire_buff, wire_elements.size ()),
                pair<short*, size_t> (filled_buff, filled_elements.size ()));
}

MeshBuilder::MeshNode* MeshBuilder::add_vertex (const Math::Vector4& pt)
{
  float q = quantize (pt);

  auto lb = _vertex_map.lower_bound (q - _threshold);
  auto ub = _vertex_map.upper_bound (q + _threshold);

  for (; lb != ub; ++lb) {
    auto n = lb->second.begin ();
    auto n_end = lb->second.end ();

    for (; n != n_end; ++n) {
      float delta = (pt - (*n)->pt).length ();

      if (Math::f_between (delta, -_threshold, _threshold)) {
        return *n;
      }
    }
  }

  MeshNode* node = new MeshNode (pt);

  _vertex_map[q].push_back (node);

  return node;
}

float MeshBuilder::quantize (const Math::Vector4& pt)
{
  float x = pt.x + pt.y + pt.z;

  return copysign (_threshold
                   * floorf (Math::f_abs (x) / _threshold + 0.5), x);
}

} /* namespace GUI */
