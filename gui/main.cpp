#include <iostream>
#include <functional>

#include <GL/glew.h>

#include "main.h"
#include "csg/exceptions.h"

#include "shader.h"
#include "gpu_pipeline.h"
#include "math/matrix4.h"
#include "math/vector3.h"

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
  : _run (false)
{
  _window = SDL_CreateWindow ("SimpleCSG", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
  model.scale (0.5, 0.5, 0.5);

  Math::Matrix4 view;
  view.look_at (Math::Vector3 (0.0, 0.0, 5.0),
                Math::Vector3 (0.0, 0.0, -1.0),
                Math::Vector3 (0.0, 1.0, 0.0));
  gpu.set_view_transform (view);

  Math::Vector4 color (1.0, 0.0, 0.0, 1.0);

  Math::Matrix4 proj;
  proj.ortho_projection (-5, 5, -5, 5, -10, 0);
  gpu.set_projection_transform (proj);

  const Shader::InputDef* pos_input = shader.get_input ("vertex");
  const Shader::InputDef* color_input = shader.get_input ("color");

  SDL_Event evt;

  while (_run) {
    gpu.clear ();

    gpu.bind_shader_input (vbo, *pos_input);
    gpu.bind_shader_input (color.data (), *color_input);
    ibo.bind ();

    gpu.set_model_transform (model);

    gpu.draw_elements (GL_LINES, 12, 0);
    //gpu.draw (GL_TRIANGLES, 6);

    SDL_GL_SwapWindow (_window);
    SDL_WaitEvent (&evt);

    switch (evt.type) {
      case SDL_QUIT:
        _run = false;
        break;
    }
  }
}

void Main::handle_keyboard (int key, int scancode, int action, int mods)
{
  cout << "Keyboard" << endl;
}

} /* namespace GUI */

int main (int argc, char** argv)
{
  GUI::Main m (argc, argv);

  m.run ();

  return 0;
}
