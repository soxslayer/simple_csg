#include "pixel.h"
#include "math/vector3.h"

namespace CSG
{

Pixel::Pixel ()
  : center (), size (0)
{
}

Pixel::Pixel (const Math::Vector2& center, float size)
  : center (center), size (size)
{
}

Pixel::Pixel (const BRect& b)
{
  Math::Vector2 c = b.center ();
  center = Math::Vector2 (c.x, c.y);

  Math::Vector2 s = b.size ();

  size = s.x;
  if (s.y > size) size = s.y;
}

} /* namespace CSG */



std::ostream& operator<< (std::ostream& os, const CSG::Pixel& p)
{
  os << p.center << ' ' << p.size;

  return os;
}
