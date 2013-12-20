#ifndef __BRECT__
#define __BRECT__

#include <ostream>

#include "math/vector2.h"

namespace CSG
{

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
  const BRect& operator= (const BRect& b);

  bool intersects (const Math::Vector2& v) const;
  Math::Vector2 size () const;
  Math::Vector2 center () const;

private:
  Math::Vector2 _lb;
  Math::Vector2 _ub;
};

} /* namespace CSG */

#endif /* __BRECT__ */
