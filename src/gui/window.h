#ifndef __WINDOW__
#define __WINDOW__

#include <functional>
#include <memory>

#include "math/vector.h"

namespace GUI
{

class Window
{
public:
  struct Event
  {
    typedef enum
    {
      None,
      Rotate,
      Zoom,
      Keyboard,
      Wireframe,
      BoundingBoxes,
      Redraw,
      ResetView,
      Quit
    } Type;

    Event () : type (Event::None) { }

    Type type;
    Math::Vector2<int> coord;
    Math::Vector2<int> rel_coord;
    float zoom;
    char key;
  };

  typedef std::function<void (const Event&)> EventHandler;
  typedef std::unique_ptr<Window> Ptr;

  virtual ~Window ();

  virtual Math::Vector2<int> get_size () const = 0;
  virtual bool render () const = 0;
  virtual void process_events (EventHandler handler) = 0;

  static Ptr create (int width, int height);
};

} /* namespace GUI */

#endif /* __WINDOW__ */
