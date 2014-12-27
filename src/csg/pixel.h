#ifndef __PIXEL__
#define __PIXEL__

#include <ostream>

#include "math/vector2.h"

namespace Math
{
class Vector2;
}

namespace CSG
{

class BRect;

class Pixel
{
public:
  Math::Vector2 center;
  float size;

  Pixel ();
  Pixel (const Math::Vector2& center, float size);
  Pixel (const BRect& b);
};

} /* namespace CSG */

std::ostream& operator<< (std::ostream& os, const CSG::Pixel& p);

#endif /* __PIXEL__ */
