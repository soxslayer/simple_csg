#include "debug_mesh.h"

#include "mesh.h"

namespace GUI
{

DebugMesh::DebugMesh(Mesh& mesh)
  : _mesh(mesh), _item(0), _n_items(1)
{
}

const DebugMesh& DebugMesh::operator++()
{
  ++_item;

  return *this;
}

const DebugMesh& DebugMesh::operator--()
{
  if (_item > 0)
    --_item;

  return *this;
}

void DebugMesh::draw(const Params& params)
{
  _mesh.draw(params, _item, 0);

  Math::Vector4<float> old_color = _mesh.get_color();
  _mesh.set_color(get_color());
  _mesh.draw(params, _n_items, _item);

  _mesh.set_color(old_color);
  _mesh.draw(params, -1, _item + _n_items);
}

} /* namespace GUI */
