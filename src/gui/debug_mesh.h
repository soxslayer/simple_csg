#ifndef __DEBUG_MESH__
#define __DEBUG_MESH__

#include "drawable.h"

namespace GUI
{

class Mesh;

class DebugMesh : public Drawable
{
public:
  DebugMesh(Mesh& mesh);

  virtual void draw(const Params& params);

  const DebugMesh& operator++();
  const DebugMesh& operator--();

private:
  Mesh& _mesh;
  int _item;
  int _n_items;
};

} /* namespace GUI */

#endif /* __DEBUG_MESH__ */
