#ifndef __BRECT__
#define __BRECT__

#include <ostream>

#include "math/vector2.h"

namespace CSG
{

class Pixel;

class BRect
{
public:
  const Math::Vector2& lb;
  const Math::Vector2& ub;

  BRect ();
  BRect (const Math::Vector2& center, float dim_x, float dim_y);
  BRect (float dim_x, float dim_y);
  BRect (const Math::Vector2& v1, const Math::Vector2& v2);
  BRect (const BRect& b);
  BRect (const Pixel& p);
  const BRect& operator= (const BRect& b);

  bool intersects (const Math::Vector2& v) const;
  Math::Vector2 size () const;
  Math::Vector2 center () const;

  static BRect combination (const BRect& r1, const BRect& r2);

private:
  Math::Vector2 _lb;
  Math::Vector2 _ub;
};

} /* namespace CSG */

std::ostream& operator<< (std::ostream& os, const CSG::BRect& b);

#endif /* __BRECT__ */
