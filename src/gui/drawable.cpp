#include "util/debug.h"
#include "drawable.h"
#include "gpu_pipeline.h"

namespace GUI
{

Drawable::Drawable ()
  : _color ({ 1.0, 1.0, 1.0, 0.0 })
{
}

void Drawable::set_color (const Math::Vector4<float>& color)
{
  set_color (color[0], color[1], color[2], color[3]);
}

void Drawable::set_color (float r, float g, float b, float a)
{
  _color[0] = r;
  _color[1] = g;
  _color[2] = b;
  _color[3] = a;
}

void Drawable::draw (const Params& params)
{
  GPUPipeline::instance ().bind_shader_input (_color.data (), "color");
}

} /* namespace GUI */
