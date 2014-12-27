#include "config.h"

#include <iostream>
#include <functional>

#if defined(HAVE_GL)
# include <GL/glew.h>
# include <SDL.h>
#elif defined(HAVE_LOAX)
# include <loax_client_gl2.h>
#endif

#include "sdl_window.h"
#include "csg/exceptions.h"

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



SDLWindow::SDLWindow (int width, int height)
	: _size ({ width, height })
{
  _window = SDL_CreateWindow ("SimpleCSG", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, width, height,
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
}

SDLWindow::~SDLWindow ()
{
  SDL_GL_DeleteContext (_gl_context);
  SDL_DestroyWindow (_window);
}

Math::Vector2<int> SDLWindow::get_size () const
{
	return _size;
}

bool SDLWindow::render () const
{
	SDL_GL_SwapWindow (_window);

	return true;
}

void SDLWindow::process_events (EventHandler handler)
{
  SDL_Event sdl_evt;
  Event evt;

  if (SDL_PollEvent(&sdl_evt)) {
    switch (sdl_evt.type) {
      case SDL_KEYDOWN:
#define GET_SYM sdl_evt.key.keysym.sym
        if (GET_SYM == SDLK_i) {
          evt.type = Event::Zoom;

          evt.zoom = .1;
        }
        else if (GET_SYM == SDLK_o) {
          evt.type = Event::Zoom;

          evt.zoom = -.1;
        }
        else if (GET_SYM == SDLK_q)
          evt.type = Event::Quit;
        else if (GET_SYM == SDLK_w)
          evt.type = Event::Wireframe;
        else if (GET_SYM == SDLK_b)
          evt.type = Event::BoundingBoxes;
        else if (GET_SYM == SDLK_r)
          evt.type = Event::ResetView;
        else {
          evt.type = Event::Keyboard;

#define GET_MOD sdl_evt.key.keysym.mod
          if (GET_SYM == SDLK_EQUALS)
            evt.key = keysym(GET_MOD, '=', '+');
          else if (GET_SYM == SDLK_MINUS)
            evt.key = keysym(GET_MOD, '-', '_');
          else if (GET_SYM == SDLK_COMMA)
            evt.key = keysym(GET_MOD, ',', '<');
          else if (GET_SYM == SDLK_PERIOD)
            evt.key = keysym(GET_MOD, '.', '>');
#undef GET_MOD
        }
#undef GET_SYM

        break;

      case SDL_MOUSEMOTION:
        if (sdl_evt.motion.state & SDL_BUTTON_LMASK) {
          evt.type = Event::Rotate;

          evt.rel_coord[0] = sdl_evt.motion.xrel;
          evt.rel_coord[1] = sdl_evt.motion.yrel;
        }

        break;

      case SDL_WINDOWEVENT:
        evt.type = Event::Redraw;

        break;

      case SDL_QUIT:
        evt.type = Event::Quit;

        break;
    }
  }

  if (evt.type != Event::None)
    handler (evt);
}

char SDLWindow::keysym(int mod, char non_shift, char shift)
{
  if (mod & KMOD_SHIFT)
    return shift;

  return non_shift;
}

} /* namespace GUI */
