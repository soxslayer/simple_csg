#ifndef __LOAX_WINDOW__
#define __LOAX_WINDOW__

#include <loax_client.h>

#include "window.h"
#include "math/vector2.h"

namespace GUI
{

class LOAXWindow : public Window
{
public:
  LOAXWindow ();
  ~LOAXWindow ();

  Math::Vector2<int> get_size () const;
  bool render () const;
  void process_events (EventHandler handler);

private:
  Math::Vector2<int> _size;
  float _hypo;
  loax_client_t* _client;
  Math::Vector2<int> _last_coord;
  float _last_zoom;
  int _num_last;

  void process_loax_events (Event& evt);
  void calc_coord (const loax_eventtouch_t& evt,
                   Math::Vector2<int>& vect) const;
  float calc_zoom (const loax_eventtouch_t& evt) const;
};

} /* namespace GUI */

#endif /* __LOAX_WINDOW__ */
