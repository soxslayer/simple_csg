#ifndef __SURFACE__
#define __SURFACE__

#include <functional>

#include "brect.h"
#include "pixel.h"
#include "math/vector2.h"

namespace CSG
{

class Surface
{
public:
  typedef std::function<void (const Pixel&, bool)> TraverseCallback;
  virtual ~Surface ();

  void traverse (TraverseCallback callback, double resolution) const;
  bool test_point (float x, float y) const { return density (x, y) >= 0; }

  float density (const Math::Vector2& v) const;
  virtual float density (float x, float y) const = 0;
  virtual bool intersects (const BRect& b) const = 0;

protected:
  BRect _brect;

  Surface (const BRect& b);
  Surface (float x, float y);

private:
  bool visit (TraverseCallback callback, const Pixel& pix,
              double min_size) const;
};

class CircleSurface : public Surface
{
public:
  CircleSurface (float radius);
  virtual ~CircleSurface () { }

  virtual float density (float x, float y) const;
  virtual bool intersects (const BRect& b) const;

private:
  float _radius;
};

class RectangleSurface : public Surface
{
public:
  RectangleSurface (float x, float y);
  ~RectangleSurface () { }

  virtual float density (float x, float y) const;
  virtual bool intersects (const BRect& b) const { return false; }
};

} /* namespace CSG */

#endif /* __SURFACE__ */
