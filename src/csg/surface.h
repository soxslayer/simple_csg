#ifndef __SURFACE__
#define __SURFACE__

#include <functional>

#include "brect.h"
#include "pixel.h"
#include "math/vector2.h"
#include "math/matrix2.h"

namespace CSG
{

class Surface
{
public:
  typedef std::function<bool (const Pixel&, bool)> TraverseCallback;

  virtual ~Surface ();

  void traverse (TraverseCallback callback, float resolution) const;
  bool test_point (float x, float y) const { return density (x, y) >= 0; }
  bool test_point (const Math::Vector2& v) const
  { return test_point (v[0], v[1]); }
  const BRect& bounds () const { return _brect; }
  const Math::Matrix2& transform () const { return _transform; }
  Math::Matrix2& transform () { return _transform; }

  float density (const Math::Vector2& v) const { return density (v[0], v[1]); }
  virtual float density (float x, float y) const = 0;
  virtual bool intersects (const BRect& b) const = 0;

protected:
  BRect _brect;

  Surface (const BRect& b);
  Surface (float x, float y);

private:
  Math::Matrix2 _transform;
  Math::Matrix2 _itransform;
  int _hash;

  bool visit (TraverseCallback callback, const Pixel& pix,
              float min_size) const;
  const Math::Matrix2& itransform ();
};

class TransformSurface : public Surface
{
public:
  virtual ~TransformSurface ();
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
  virtual bool intersects (const BRect& b) const;
};

class UnionSurface : public Surface
{
public:
  UnionSurface (const Surface& s1, const Surface& s2);

  virtual float density (float x, float y) const;
  virtual bool intersects (const BRect& b) const;

private:
  const Surface& _s1;
  const Surface& _s2;
};

} /* namespace CSG */

#endif /* __SURFACE__ */
