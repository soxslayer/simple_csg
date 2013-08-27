#ifndef __MAIN__
#define __MAIN__

#include <SDL.h>

#include "math/matrix4.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LEFT_CLIP -10
#define RIGHT_CLIP 10
#define BOTTOM_CLIP -10
#define TOP_CLIP 10
#define NEAR_CLIP 0
#define FAR_CLIP -20
#define ZOOM_RATE 0.9

namespace GUI
{

class SDLContext
{
public:
  ~SDLContext ();

private:
  SDLContext ();
  SDLContext (const SDLContext& c) { }
  const SDLContext& operator= (const SDLContext& c) { return *this; }

  static SDLContext _context;
};

class Main
{
public:
  Main (int argc, char** argv);
  ~Main ();

  void run ();

private:
  SDL_Window* _window;
  SDL_GLContext _gl_context;
  bool _run;
  Math::Matrix4 _view;
  Math::Matrix4 _view_inv;
  Math::Matrix4 _proj;
  Math::Matrix4 _proj_inv;
  float _zoom;
  float _aspect;
  bool _redraw;

  void handle_keyboard (const SDL_KeyboardEvent& evt);
  void handle_mouse_motion (const SDL_MouseMotionEvent& evt);
  void reset_view ();
  void calculate_projection ();
};

} /* namespace GUI */

#endif /* __MAIN__ */
