#include <math.h>

#include "volume.h"
#include "math/vector4.h"
#include "math/quaternion.h"
#include "math/util.h"
#include <iostream>
using namespace std;

namespace CSG
{

Volume::Volume ()
{
}

void Volume::oct_traverse (TraverseCallback callback, double resolution) const
{
#if 0
  Voxel vox (bbox ());
  double s4 = vox.size / 4;

  for (int i = 0; i < 8; ++i) {
    Math::Vector3 pt;

    pt.x = i & 1 ? -s4 : s4;
    pt.y = i & 2 ? -s4 : s4;
    pt.z = i & 4 ? -s4 : s4;

    Voxel v (pt, vox.size / 2);

    if (!visit (callback, Voxel (pt, vox.size / 2), resolution))
      return;
  }
#endif
}

#if 0
void Volume::set_transform (const Math::Matrix4& transform)
{
  _wtm_transform = transform;
  _mtw_transform = transform.inverted ();

  invalidate ();
}

const Math::Vector3& get_size () const
{
  return to_world_coordinates (_w_size, 0.0);
}

void set_size (const Math::Vector3& m_size)
{
  _m_size = m_size;
}

Math::Vector3 Volume::to_model_coordinates (const Math::Vector3& w_coord,
                                            float w) const
{
  Math::Vector4 pt (w_coord);

  pt.w = w;
  pt *= _wtm_transform;

  return Math::Vector3 (pt.x, pt.y, pt.z);
}

Math::Vector3 Volume::to_world_coordinates (const Math::Vector3& m_coord,
                                            float w) const
{
  Math::Vector4 pt (w_coord);

  pt.w = w;
  pt *= _wtm_transform;

  return Math::Vector3 (pt.x, pt.y, pt.z);
}

bool Volume::visit (TraverseCallback callback, const Voxel& vox,
                    double min_size) const
{
  int num_inside = 0;
  double s2 = vox.size / 2;
  double s4 = vox.size / 4;

  for (int i = 0; i < 8; ++i) {
    Math::Vector3 n_pt = vox.center;

    n_pt.x += i & 1 ? -s2 : s2;
    n_pt.y += i & 2 ? -s2 : s2;
    n_pt.z += i & 4 ? -s2 : s2;

    num_inside += test_point (n_pt) ? 1 : 0;
  }

  if (num_inside == 0)
    return true;
  else if (num_inside == 8)
    return callback (vox, true);
  else if (num_inside < 8 && vox.size <= min_size)
    return callback (vox, false);

  for (int i = 0; i < 8; ++i) {
    Voxel n_vox = vox;

    n_vox.center.x += i & 1 ? -s4 : s4;
    n_vox.center.y += i & 2 ? -s4 : s4;
    n_vox.center.z += i & 4 ? -s4 : s4;
    n_vox.size = s2;

    if (!visit (callback, n_vox, min_size))
      return false;
  }

  return true;
}
#endif



RotateSurfaceVolume::RotateSurfaceVolume (const Surface& surface)
  : _surface (surface)
{
}

double RotateSurfaceVolume::density (const Math::Vector3& pt) const
{
  if (pt.length () == 0)
    return _surface.density (pt.x, pt.y);

  Math::Vector3 yz_pt (0.0, pt.y, pt.z);
  float xy_angle = Math::Vector3::y_axis.angle_between (yz_pt);

  Math::Matrix4 rot_m;
  rot_m.rotate (xy_angle, 0.0, 0.0);

  Math::Vector4 test_pt = (Math::Vector4)pt * rot_m;

  return _surface.density (test_pt.x, test_pt.y);
}



#if 0
SphereVolume::SphereVolume (double radius)
  : Volume (BBox (radius * 2 + 0.001, radius * 2 + 0.001, radius * 2 + 0.001)),
    _radius (radius), _hash (0)
{
}

double SphereVolume::density (const Math::Vector3& pt) const
{
  return _radius - pt.length ();
}



BoxVolume::BoxVolume (double x, double y, double z)
  : Volume (BBox (x + 0.001, y + 0.001, z + 0.001)), _hash (0), _dim (x, y, z)
{
}

BoxVolume::BoxVolume (double size)
  : Volume (BBox (size + 0.001, size + 0.001, size + 0.001)), _hash (0),
    _dim (size, size, size)
{
}

double BoxVolume::density (const Math::Vector3& pt) const
{
  double r = pt.x + _dim.size.x / 2;
  double x2 = _dim.size.x / 2 - pt.x;
  double y1 = pt.y + _dim.size.y / 2;
  double y2 = _dim.size.y / 2 - pt.y;
  double z1 = pt.z + _dim.size.z / 2;
  double z2 = _dim.size.z / 2 - pt.z;

  if (r > x2) r = x2;
  if (r > y1) r = y1;
  if (r > y2) r = y2;
  if (r > z1) r = z1;
  if (r > z2) r = z2;

  return r;
}



CylinderVolume::CylinderVolume (double radius, double height, MajorAxis axis)
  : _radius (radius), _height (height), _axis (axis), _hash (0)
{
  BBox& bb = bbox ();

  switch (axis) {
    case X_AXIS:
      bb.size.x = height + 0.001;
      bb.size.y = _radius + 0.001;
      bb.size.z = _radius + 0.001;

      break;

    case Y_AXIS:
      bb.size.x = _radius + 0.001;
      bb.size.y = height + 0.001;
      bb.size.z = _radius + 0.001;

      break;

    case Z_AXIS:
      bb.size.x = _radius + 0.001;
      bb.size.y = _radius + 0.001;
      bb.size.z = height + 0.001;

      break;
  }
}

double CylinderVolume::density (const Math::Vector3& pt) const
{
  Math::Vector3 n_pt = pt;
  double d;

  if (_axis == X_AXIS) {
    Math::Quaternion rot (Math::Vector3 (0.0, 1.0, 0.0), 90);
    n_pt *= Math::Matrix4 (rot);
  }
  else if (_axis == Y_AXIS) {
    Math::Quaternion rot (Math::Vector3 (1.0, 0.0, 0.0), 90);
    n_pt *= Math::Matrix4 (rot);
  }

  if (Math::f_between (n_pt.z, - _height / 2,  _height / 2))
    d = _radius - sqrt (n_pt.x * n_pt.x + n_pt.y * n_pt.y);
  else
    d = n_pt.z > 0 ?  _height / 2 - n_pt.z :  _height / 2 + n_pt.z;

  return d;
}



IntersectionVolume::IntersectionVolume (const Volume& v1, const Volume& v2)
  : Volume (BBox::intersection (v1.bbox (), v2.bbox ())), _v1 (v1), _v2 (v2)
{
}

double IntersectionVolume::density (const Math::Vector3& pt) const
{
  double v1 = _v1.density (_v1.bbox ().center - pt - bbox ().center);
  double v2 = _v2.density (_v2.bbox ().center - pt - bbox ().center);

  return v1 < v2 ? v1 : v2;
}

int IntersectionVolume::hash () const
{
  int v1 = _v1.hash () & 0xffff;
  int v2 = _v2.hash () & 0xffff;

  return v1 << 16 | v2;
}



DifferenceVolume::DifferenceVolume (const Volume& v1, const Volume& v2)
  : Volume (BBox::difference (v1.bbox (), v2.bbox ())), _v1 (v1), _v2 (v2)
{
}

double DifferenceVolume::density (const Math::Vector3& pt) const
{
  double v1 = _v1.density (pt - _v1.bbox ().center);
  double v2 = _v2.density (pt - _v2.bbox ().center);
  double r = v1;

  if (v1 >= 0 && Math::f_abs (v2) < Math::f_abs (v1))
    r = v2;

  return r;
}

int DifferenceVolume::hash () const
{
  int v1 = _v1.hash () & 0xffff;
  int v2 = _v2.hash () & 0xffff;

  return v1 << 16 | v2;
}



CombinationVolume::CombinationVolume (const Volume& v1, const Volume& v2)
  : Volume (BBox::combination (v1.bbox (), v2.bbox ())), _v1 (v1), _v2 (v2)
{
}

double CombinationVolume::density (const Math::Vector3& pt) const
{
  return 0;
}

int CombinationVolume::hash () const
{
  int v1 = _v1.hash () & 0xffff;
  int v2 = _v2.hash () & 0xffff;

  return v1 << 16 | v2;
}
#endif

} /* namespace CSG */
