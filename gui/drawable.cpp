#include "drawable.h"
#include "gpu_pipeline.h"

namespace GUI
{

Drawable::Drawable ()
  : _color (1.0, 1.0, 1.0, 0.0)
{
}

void Drawable::set_color (float r, float g, float b, float a)
{
  _color.x = r;
  _color.y = g;
  _color.z = b;
  _color.w = a;
}

void Drawable::draw ()
{
  GPUPipeline::instance ().bind_shader_input (_color.data (), "color");
}

} /* namespace GUI */
