#include <utility>

#include "mesh.h"

#include "tango_palette.h"

namespace GUI
{

Mesh::Mesh(Mesh&& m)
  : _verticies(std::move(m._verticies)),
    _wireframe(std::move(m._wireframe)),
    _filled(std::move(m._filled))
{
}

Mesh::Mesh(const std::vector<float>& verticies,
           const std::vector<short>& wireframe_elements,
           const std::vector<short>& filled_elements)
{
  _verticies.alloc(verticies.data(), static_cast<size_t>(verticies.size()));
  _wireframe.alloc(wireframe_elements.data(),
                   static_cast<size_t>(wireframe_elements.size()));
  _filled.alloc(filled_elements.data(),
                static_cast<size_t>(filled_elements.size()));
}

const Mesh& Mesh::operator=(Mesh&& m)
{
  if (&m != this) {
    _verticies = std::move(m._verticies);
    _wireframe = std::move(m._wireframe);
    _filled = std::move(m._filled);
  }

  return *this;
}

void Mesh::draw(const Params& params)
{
  draw(params, -1, 0);
}

void Mesh::draw(const Params& params, int n_elements, int offset)
{
  if (n_elements == 0)
    return;

  Drawable::draw(params);

  GUI::GPUPipeline& gpu = GUI::GPUPipeline::instance();

  gpu.bind_shader_input(_verticies, "vertex");

  if (params.wireframe)
    draw_elements(_wireframe, GL_LINES, 6, n_elements, offset);
  else
    draw_elements(_filled, GL_TRIANGLES, 3, n_elements, offset);
}

void Mesh::draw_elements(GPUPipeline::ElementBuffer& elements,
                         int item_type, int item_size, int n_items, int offset)
{
  if (n_items == 0)
    return;

  int b = offset * item_size;
  int e;
  if (n_items < 0)
    e = elements.get_count();
  else
    e = b + n_items * item_size;

  if (b < 0)
    b = 0;

  if (e > elements.get_count())
    e = elements.get_count();

  elements.bind();
  GPUPipeline::instance().draw_elements(item_type, e - b, b);
}

} /* namespace GUI */
