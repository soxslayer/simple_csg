#ifndef __DRAWABLE__
#define __DRAWABLE__

#include "math/vector.h"

namespace GUI
{

class Drawable
{
public:
  struct Params
  {
    Params ()
      : bounding_boxes (false),
        wireframe (false)
    { }

    bool bounding_boxes;
    bool wireframe;
  };

  Drawable ();
  virtual ~Drawable () { }

  const Math::Vector4<float>& get_color () const { return _color; }
  void set_color (const Math::Vector4<float>& color);

  virtual void set_color (float r, float g, float b, float a);
  virtual void draw (const Params& params);

private:
  Math::Vector4<float> _color;
};

} /* namespace GUI */

#endif /* __DRAWABLE__ */
