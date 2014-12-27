#include "util/debug.h"
#include "config.h"

#include <iostream>
#include <functional>

#include "main.h"

#include "debug_mesh.h"
#include "shader.h"
#include "gpu_pipeline.h"
#include "tango_palette.h"
#include "volume_mesh.h"
#include "csg/exceptions.h"
#include "csg/sphere_volume.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/vector.h"
#include "math/util.h"
#include "util/signal_adapter.h"

using namespace std;

namespace GUI
{

Main::Main (int argc, char** argv)
  : _run (false), _zoom (0.1)
{
}

void Main::run (const Window::Ptr& window)
{
  Shader shader ("src/gui/simple.vert", "src/gui/simple.frag");
  GPUPipeline& gpu = GPUPipeline::instance ();
  CSG::SphereVolume v1 (1);
  VolumeMesh m1(v1);

  gpu.set_shader (shader);
  gpu.enable_face_culling();
  gpu.enable_depth_buffer();

  Math::Matrix4<float> model;

  Math::Vector2<int> dim = window->get_size ();
  _aspect = dim[0] / dim[1];

  reset_view ();
  calculate_projection ();

  _redraw = true;
  _run = true;

  Util::SignalAdapter::register_handler (SIGINT,
      [&] (int sig) { handle_signal (sig); });

  while (_run) {
    if (_redraw) {
      gpu.clear (0, 0, 0, 1);

      gpu.set_model_transform (model);
      gpu.set_view_transform (_view);
      gpu.set_projection_transform (_proj);
      Math::Vector2<int> window_size = window->get_size ();
      gpu.set_viewport (window_size[0], window_size[1]);

      m1.draw(_draw_params);

      if (!window->render ())
        _run = false;

      _redraw = false;
    }

    window->process_events (
      [&] (const GUI::Window::Event& evt) { _redraw = handle_event (evt); });
  }
}

bool Main::handle_event (const GUI::Window::Event& evt)
{
  switch (evt.type) {
    case GUI::Window::Event::Rotate: {
      /* Undo the view transform because we want to rotate about the x and y
       * axis as seen by the user */
      Math::Vector4<float> x_axis = Math::Vector4<float>::axis (0) * _view_inv;
      Math::Vector4<float> y_axis = Math::Vector4<float>::axis (1) * _view_inv;

      Math::Quaternion<float> rot_x (x_axis, evt.rel_coord[1] * .25);
      Math::Quaternion<float> rot_y (y_axis, evt.rel_coord[0] * .25);

      rot_x *= rot_y;
      Math::Matrix4<float> rot_m (rot_x);
      rot_m.transpose ();

      _view = rot_m * _view;
      _view_inv = _view.inverted ();

      return true;
    }

    case GUI::Window::Event::Zoom:
      if (evt.zoom != 0) {
        _zoom *= 1 + 2 * evt.zoom;

        calculate_projection ();
      }

      return true;

    case GUI::Window::Event::Wireframe:
      _draw_params.wireframe = !_draw_params.wireframe;

      return true;

    case GUI::Window::Event::BoundingBoxes:
      _draw_params.bounding_boxes = !_draw_params.bounding_boxes;

      return true;

    case GUI::Window::Event::Redraw:
      return true;

    case GUI::Window::Event::ResetView:
      reset_view ();

      return true;

    case GUI::Window::Event::Quit:
      _run = false;

      break;

    default:
      break;
  }

  return false;
}

void Main::reset_view ()
{
  _view.look_at (Math::Vector3<float> ({ 0.0, 0.0, 10.0 }),
                 Math::Vector3<float> ({ 0.0, 0.0, -1.0 }),
                 Math::Vector3<float> ({ 0.0, 1.0, 0.0 }));
  _view_inv = _view.inverted ();
}

void Main::calculate_projection ()
{
  _proj.ortho_projection (-_aspect, _aspect, -1, 1, NEAR_CLIP, FAR_CLIP);
  _proj.scale (_zoom, _zoom, _zoom);
  _proj_inv = _proj.inverted ();
}

void Main::handle_signal (int sig)
{
  if (sig == SIGINT)
    _run = false;
}

} /* namespace GUI */



int main (int argc, char** argv)
{
  GUI::Main m (argc, argv);
  GUI::Window::Ptr window = GUI::Window::create (800, 600);

  m.run (window);

  return 0;
}
