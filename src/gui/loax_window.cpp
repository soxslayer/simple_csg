#include "loax_window.h"

#include "csg/exceptions.h"

namespace GUI
{

LOAXWindow::LOAXWindow ()
{
  _client = loax_client_new ();

  if (_client == nullptr)
    throw CSG::Exception ("LOAX client creation failed");

  _num_last = 0;

  int w, h;

  loax_client_size (_client, &w, &h);

  _size.x = (float)w;
  _size.y = (float)h;
  _hypo = _size.length ();
}

LOAXWindow::~LOAXWindow ()
{
  loax_client_delete (&_client);
}

Math::Vector2<int> LOAXWindow::get_size () const
{
  return _size;
}

bool LOAXWindow::render () const
{
  return loax_client_swapbuffers (_client);
}

void LOAXWindow::process_events (EventHandler handler)
{
  Event evt;
  loax_event_t e;

  evt.type = Event::None;

  loax_client_poll (_client, &e);

  switch (e.type) {
    case LOAX_EVENT_TOUCHMOVE: {
      calc_coord (e.event_touch, evt.coord);

      evt.rel_coord = evt.coord - _last_coord;

      _last_coord = evt.coord;

      evt.type = Event::Rotate;

      float zoom = calc_zoom (e.event_touch);

      if (_num_last == e.event_touch.count) {
        if (e.event_touch.count == 1) {
          handler (evt);
        }
        else if (e.event_touch.count == 2) {
          evt.type = Event::Zoom;

          evt.zoom = (zoom - _last_zoom) / _hypo;

          handler (evt);
        }
      }

      if (e.event_touch.count == 2) {
        _last_zoom = zoom;
      }

      _num_last = e.event_touch.count;

      break;
    }

    case LOAX_EVENT_TOUCHDOWN:
      calc_coord (e.event_touch, _last_coord);

      break;
  }
}

void LOAXWindow::calc_coord (const loax_eventtouch_t& evt,
                             Math::Vector2<int>& vect) const
{
  vect.x = 0;
  vect.y = 0;

  for (int i = 0; i < evt.count; ++i) {
    vect.x += evt.coord[i].x;
    vect.y += evt.coord[i].y;
  }

  vect.x /= evt.count;
  vect.y /= evt.count;
}

float LOAXWindow::calc_zoom (const loax_eventtouch_t& evt) const
{
  if (evt.count < 2)
    return 0;

  Math::Vector2<int> pt0 ({ evt.coord[0].x, evt.coord[0].y });
  Math::Vector2<int> pt1 ({ evt.coord[1].x, evt.coord[1].y });

  return (pt0 - pt1).length ();
}

} /* namespace GUI */
