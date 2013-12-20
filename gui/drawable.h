#ifndef __DRAWABLE__
#define __DRAWABLE__

#include "math/vector4.h"

namespace GUI
{

class Drawable
{
public:
  Drawable ();
  virtual ~Drawable () { }

  const Math::Vector4& get_color () const { return _color; }
  void set_color (const Math::Vector4& color) { _color = color; }
  void set_color (float r, float g, float b, float a);

  virtual void draw ();

private:
  Math::Vector4 _color;
};

} /* namespace GUI */

#endif /* __DRAWABLE__ */
