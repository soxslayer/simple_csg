#ifndef __SDL_WINDOW__
#define __SDL_WINDOW__

#include <SDL.h>

#include "window.h"

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

class SDLWindow : public Window
{
public:
  SDLWindow (int width, int height);
  ~SDLWindow ();

	Math::Vector2<int> get_size () const;
	bool render () const;
	void process_events (EventHandler handler);

private:
  SDL_Window* _window;
  SDL_GLContext _gl_context;
	Math::Vector2<int> _size;

  char keysym(int mod, char non_shift, char shift);
};

} /* namespace GUI */

#endif /* __SDL_WINDOW__ */
