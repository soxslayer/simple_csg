#include <iostream>
#include <functional>

#include <GL/glew.h>

#include "main.h"
#include "csg/exceptions.h"

#include "shader.h"
#include "gpu_pipeline.h"
#include "math/matrix4.h"
#include "math/vector3.h"
#include "math/quaternion.h"
#include "math/util.h"

using namespace std;

namespace GUI
{

SDLContext::~SDLContext ()
{
  SDL_Quit ();
}

SDLContext::SDLContext ()
{
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    throw CSG::Exception ("SDL failed to init");
}

SDLContext SDLContext::_context;



Main::Main (int argc, char** argv)
  : _run (false), _zoom (10.0)
{
  _window = SDL_CreateWindow ("SimpleCSG", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!_window)
    throw CSG::Exception ("Failed to create SDL window");

  _gl_context = SDL_GL_CreateContext (_window);

  SDL_GL_SetSwapInterval (1);

  if (glewInit () != GLEW_OK) {
    SDL_GL_DeleteContext (_gl_context);
    SDL_DestroyWindow (_window);

    throw CSG::Exception ("GLEW failed to init");
  }

  _run = true;
  _aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
}

Main::~Main ()
{
  SDL_GL_DeleteContext (_gl_context);
  SDL_DestroyWindow (_window);
}

void Main::run ()
{
  float t1[] = {
    1.0, 1.0, 0.0, 1.0,
    1.0, -1.0, 0.0, 1.0,
    -1.0, 1.0, 0.0, 1.0,
    -1.0, -1.0, 0.0, 1.0
  };

  short e1[] = {
    //0, 1, 2, 1, 2, 3
    0, 1, 1, 2, 2, 0, 1, 2, 2, 3, 3, 1
  };

  Shader shader ("../gui/simple.vert", "../gui/simple.frag");
  GPUPipeline& gpu = GPUPipeline::instance ();

  gpu.set_shader (shader);

  GPUPipeline::VertexBuffer vbo;
  vbo.alloc (t1, sizeof (t1));

  GPUPipeline::ElementBuffer ibo;
  ibo.alloc (e1, sizeof (e1));

  Math::Matrix4 model;
  //model.scale (0.25, 0.25, 0.25);

  reset_view ();
  calculate_projection ();

  Math::Vector4 color (1.0, 0.0, 0.0, 1.0);

  const Shader::InputDef* pos_input = shader.get_input ("vertex");
  const Shader::InputDef* color_input = shader.get_input ("color");

  _redraw = true;
  SDL_Event evt;

  while (_run) {
    if (_redraw) {
      gpu.clear ();

      gpu.set_model_transform (model);
      gpu.set_view_transform (_view);
      gpu.set_projection_transform (_proj);

      gpu.bind_shader_input (vbo, *pos_input);
      gpu.bind_shader_input (color.data (), *color_input);
      ibo.bind ();

      gpu.set_model_transform (model);
      gpu.set_view_transform (_view);

      gpu.draw_elements (GL_LINES, 12, 0);
      //gpu.draw_elements (GL_TRIANGLES, 6, 0);

      _redraw = false;
    }

    SDL_GL_SwapWindow (_window);
    SDL_WaitEvent (&evt);

    switch (evt.type) {
      case SDL_QUIT: _run = false; break;
      case SDL_KEYDOWN:
      case SDL_KEYUP: handle_keyboard (evt.key); break;
      case SDL_MOUSEMOTION: handle_mouse_motion (evt.motion); break;
    }
  }
}

void Main::handle_keyboard (const SDL_KeyboardEvent& evt)
{
  if (evt.state == SDL_PRESSED) {
    switch (evt.keysym.sym) {
      case SDLK_q: _run = false; break;
      case SDLK_i: _zoom *= ZOOM_RATE; calculate_projection (); break;
      case SDLK_o: _zoom /= ZOOM_RATE; calculate_projection (); break;
    }

    _redraw = true;
  }
}

void Main::handle_mouse_motion (const SDL_MouseMotionEvent& evt)
{
  switch (evt.state) {
    case SDL_BUTTON_LMASK: {
      /* Undo the view transform because we want to rotate about the x and y
       * axis as seen by the user */
      Math::Vector4 x_axis = Math::Vector4::x_axis * _view_inv;
      Math::Vector4 y_axis = Math::Vector4::y_axis * _view_inv;

      Math::Quaternion rot_x (x_axis, evt.yrel);
      Math::Quaternion rot_y (y_axis, evt.xrel);

      rot_x *= rot_y;
      Math::Matrix4 rot_m (rot_x);
      rot_m.transpose ();

      _view = rot_m * _view;
      _view_inv = _view.inverted ();

      _redraw = true;

      break;
    }

    case SDL_BUTTON_RMASK: {
      /* convert number of pixels into clip space then undo projection
       * transform to get translation in view space */
      Math::Vector4 trans (
        Math::f_interpolate (evt.xrel, 0, 0, WINDOW_WIDTH, 2),
        Math::f_interpolate (-evt.yrel, 0, 0, WINDOW_HEIGHT, 2), 0.0, 1.0);
      trans *= _proj_inv;
      /* we don't want to undo the z transform */
      trans.z = 0;
      _view.translate (trans);
      _view_inv = _view.inverted ();

      _redraw = true;

      break;
    }
  }
}

void Main::reset_view ()
{
  _view.look_at (Math::Vector3 (0.0, 0.0, 10.0),
                 Math::Vector3 (0.0, 0.0, -1.0),
                 Math::Vector3 (0.0, 1.0, 0.0));
  _view_inv = _view.inverted ();
}

void Main::calculate_projection ()
{
  _proj.ortho_projection (-_aspect * _zoom, _aspect * _zoom, -_zoom, _zoom,
                          NEAR_CLIP, FAR_CLIP);
  _proj_inv = _proj.inverted ();
}

} /* namespace GUI */



int main (int argc, char** argv)
{
  GUI::Main m (argc, argv);

  m.run ();

  return 0;
}
