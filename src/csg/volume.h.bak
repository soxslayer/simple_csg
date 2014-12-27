#ifndef __VOLUME__
#define __VOLUME__

#include <functional>

#include "voxel.h"
#include "surface.h"
#include "math/vector3.h"
#include "math/matrix4.h"

namespace CSG
{

class Volume
{
public:
  typedef std::function<bool (const Voxel&, bool)> TraverseCallback;

  Volume ();
  virtual ~Volume () { }

  void oct_traverse (TraverseCallback callback, double resolution) const;
  bool test_point (const Math::Vector3& w_coord) const
  { return density (w_coord) >= 0; }
#if 0
  const Math::Matrix4& get_wtm_transform () const { return _wtm_transform; }
  const Math::Matrix4& get_mtw_transform () const { return _mtw_transform; }
  void set_transform (const Math::Matrix4& transform);
  int get_hash () const { return _hash; }
  const Math::Vector3& get_size () const;
#endif

  virtual double density (const Math::Vector3& w_coord) const = 0;

protected:
  void invalidate () { ++_hash; }
#if 0
  void set_size (const Math::Vector3& m_size);
  Math::Vector3 to_model_coordinates (const Math::Vector3& w_coord,
                                      float w = 1.0) const;
  Math::Vector3 to_world_coordinates (const Math::Vector3& m_coord,
                                      float w = 1.0) const;
#endif

private:
  int _hash;
#if 0
  Math::Vector3 _m_size;
  Math::Matrix4 _wtm_transform;
  Math::Matrix4 _mtw_transform;

  bool visit (TraverseCallback callback, const Voxel& vox,
              double min_size) const;
#endif
};

class RotateSurfaceVolume : public Volume
{
public:
  RotateSurfaceVolume (const Surface& surface);

  virtual double density (const Math::Vector3& pt) const;

private:
  const Surface& _surface;
};

#if 0
class SphereVolume : public Volume
{
public:
  SphereVolume (double radius);

  virtual double density (const Math::Vector3& pt) const;

private:
  double _radius;
};

class BoxVolume : public Volume
{
public:
  BoxVolume (double x, double y, double z);
  BoxVolume (double size);

  virtual double density (const Math::Vector3& pt) const;

private:
  BBox _dim;
};

class CylinderVolume : public Volume
{
public:
  CylinderVolume (double radius, double height,
                  MajorAxis axis = Volume::Z_AXIS);

  virtual double density (const Math::Vector3& pt) const;

private:
  double _radius;
  double _height;
  MajorAxis _axis;
};

class IntersectionVolume : public Volume
{
public:
  IntersectionVolume (const Volume& v1, const Volume& v2);

  virtual double density (const Math::Vector3& pt) const;

private:
  const Volume& _v1;
  const Volume& _v2;
};

class DifferenceVolume : public Volume
{
public:
  DifferenceVolume (const Volume& v1, const Volume& v2);

  virtual double density (const Math::Vector3& pt) const;

private:
  const Volume& _v1;
  const Volume& _v2;
};

class CombinationVolume : public Volume
{
public:
  CombinationVolume (const Volume& v1, const Volume& v2);

  virtual double density (const Math::Vector3& pt) const;

private:
  const Volume& _v1;
  const Volume& _v2;
};
#endif

} /* namespace CSG */

#endif /* __VOLUME__ */
