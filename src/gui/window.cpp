#include "config.h"
#include "window.h"

#if defined (HAVE_SDL2)
# include "sdl_window.h"
#elif defined (HAVE_LOAX)
# include "loax_window.h"
#endif

using namespace std;

namespace GUI
{

Window::~Window ()
{
}

Window::Ptr Window::create (int width, int height)
{
#ifdef HAVE_SDL2
  return Ptr (new GUI::SDLWindow (width, height));
#elif defined(HAVE_LOAX)
  return Ptr (new GUI::LOAXWindow (width, height));
#else
# error Unsupported windowing abstraction
#endif
}

} /* namespace GUI */
