#ifndef __MAIN__
#define __MAIN__

#include <SDL.h>

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

  void handle_keyboard (int key, int scancode, int action, int mods);
};

} /* namespace GUI */

#endif /* __MAIN__ */
