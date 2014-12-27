#ifndef __MAIN__
#define __MAIN__

#include "window.h"
#include "math/matrix.h"
#include "gui/drawable.h"

#define LEFT_CLIP -10
#define RIGHT_CLIP 10
#define BOTTOM_CLIP -10
#define TOP_CLIP 10
#define NEAR_CLIP 0
#define FAR_CLIP -20
#define ZOOM_RATE 0.9

namespace GUI
{

class Main
{
public:
  Main (int argc, char** argv);

  void run (const Window::Ptr& window);

private:
  bool _run;
  Math::Matrix4<float> _view;
  Math::Matrix4<float> _view_inv;
  Math::Matrix4<float> _proj;
  Math::Matrix4<float> _proj_inv;
  float _zoom;
  float _aspect;
  bool _redraw;
  Drawable::Params _draw_params;

  bool handle_event (const GUI::Window::Event& evt);
  void reset_view ();
  void calculate_projection ();
  void handle_signal (int sig);
};

} /* namespace GUI */

#endif /* __MAIN__ */
