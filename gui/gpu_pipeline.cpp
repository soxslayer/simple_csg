#include <GL/glew.h>
#include <GL/gl.h>

#include "gpu_pipeline.h"

using namespace std;

namespace GUI
{

GPUPipeline::Buffer::Buffer (GLenum type)
  : _type (type), _count (0)
{
  glGenBuffers (1, &_handle);
}

GPUPipeline::Buffer::~Buffer ()
{
  glDeleteBuffers (1, &_handle);
}

void GPUPipeline::Buffer::alloc (const void* data, GLsizei count,
                                 GLsizeiptr item_size, GLenum usage)
{
  glBindBuffer (_type, _handle);
  glBufferData (_type, count * item_size, data, usage);
  _count = count;
}

void GPUPipeline::Buffer::set (const void* data, GLsizei count,
                               GLsizeiptr item_size, GLintptr offset)
{
  glBindBuffer (_type, _handle);
  glBufferSubData (_type, offset, count * item_size, data);
  _count = count;
}

void GPUPipeline::Buffer::bind () const
{
  glBindBuffer (_type, _handle);
}

void GPUPipeline::Buffer::unbind () const
{
  glBindBuffer (_type, 0);
}



GPUPipeline::VertexBuffer::VertexBuffer (const float* buff, GLsizei count)
  : Buffer (GL_ARRAY_BUFFER)
{
  Buffer::alloc (buff, count, sizeof (float));
}

void GPUPipeline::VertexBuffer::alloc (const float* buff, GLsizei count)
{
  Buffer::alloc (buff, count, sizeof (float));
}

void GPUPipeline::VertexBuffer::set (const float* buff, GLsizei count,
                                     GLintptr offset)
{
  Buffer::set (buff, count, sizeof (float), offset);
}



GPUPipeline::ElementBuffer::ElementBuffer (const short* buff, GLsizei count)
  : Buffer (GL_ELEMENT_ARRAY_BUFFER)
{
  Buffer::alloc (buff, count, sizeof (short));
}

void GPUPipeline::ElementBuffer::alloc (const short* buff, GLsizei count)
{
  Buffer::alloc (buff, count, sizeof (short));
}

void GPUPipeline::ElementBuffer::set (const short* buff, GLsizei count,
                                      GLintptr offset)
{
  Buffer::set (buff, count, sizeof (short), offset);
}



void GPUPipeline::set_clear_color (float r, float g, float b, float a)
{
  _clear_r = r;
  _clear_g = g;
  _clear_b = b;
  _clear_a = a;
}

void GPUPipeline::clear () const
{
  glClearColor (_clear_r, _clear_g, _clear_b, _clear_a);
  glClear (GL_COLOR_BUFFER_BIT);
}

void GPUPipeline::clear (float r, float g, float b, float a) const
{
  glClearColor (r, g, b, a);
  glClear (GL_COLOR_BUFFER_BIT);
}

const Shader& GPUPipeline::get_shader () const
{
  if (!_shader)
    throw Exception ("no shader set");

  return *_shader;
}

void GPUPipeline::set_shader (const Shader& shader)
{
  _shader_mvp_input = &shader.get_input ("mvp_transform");

  glUseProgram (shader.get_program ());

  _shader = &shader;
}

void GPUPipeline::bind_shader_input (void *data, const Shader::InputDef& input,
                                     GLsizei stride) const
{
  switch (input.def_type) {
    case Shader::InputDef::ATTRIBUTE: {
      GLenum attrib_type;
      GLint size;

      switch (input.type) {
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_VEC4:
          attrib_type = GL_FLOAT;
          break;

        default:
          throw CSG::UnsupportedException ("shader attribute type");
      }

      switch (input.type) {
        case GL_FLOAT_VEC2:
          size = 2;
          break;

        case GL_FLOAT_VEC3:
          size = 3;
          break;

        case GL_FLOAT_VEC4:
          size = 4;
          break;

        default:
          throw CSG::UnsupportedException ("shader attribute type");
      }

      glEnableVertexAttribArray (input.location);
      glVertexAttribPointer (input.location, size, attrib_type, GL_FALSE,
                             stride, data);

      break;
    }

    case Shader::InputDef::UNIFORM:
      switch (input.type) {
        case GL_FLOAT_MAT4:
          glUniformMatrix4fv (input.location, input.num, GL_FALSE,
                              (float *)data);
          break;

        case GL_FLOAT_VEC4:
          glUniform4fv (input.location, input.num, (float*)data);
          break;

        default:
          throw CSG::UnsupportedException ("shader uniform type");
      }

      break;
  }
}

void GPUPipeline::bind_shader_input (void* data, const std::string& input,
                                     GLsizei stride) const
{
  const Shader::InputDef& def = get_shader ().get_input (input);
  bind_shader_input (data, def, stride);
}

void GPUPipeline::bind_shader_input (const Buffer& buff,
                                     const Shader::InputDef& input,
                                     long offset, GLsizei stride) const
{
  if (input.def_type != Shader::InputDef::ATTRIBUTE)
    throw InputException (input.name, "invalid type");

  buff.bind ();
  bind_shader_input ((void*)offset, input, stride);
  buff.unbind ();
}

void GPUPipeline::bind_shader_input (const Buffer& buff,
                                     const std::string& input, long offset,
                                     GLsizei stride) const
{
  const Shader::InputDef& def = get_shader ().get_input (input);
  bind_shader_input (buff, def, offset, stride);
}

void GPUPipeline::draw (GLenum mode, GLsizei count)
{
  if (_shader_mvp_input) {
    calculate_vp_transform ();

    Math::Matrix4 mvp_transform = _m_transform * _vp_transform;
    bind_shader_input (mvp_transform.data (), *_shader_mvp_input);
  }

  glDrawArrays (mode, 0, count);
}

void GPUPipeline::draw_elements (GLenum mode, GLsizei count, long offset,
                                 GLenum type)
{
  if (_shader_mvp_input) {
    calculate_vp_transform ();

    Math::Matrix4 mvp_transform = _m_transform * _vp_transform;
    bind_shader_input (mvp_transform.data (), *_shader_mvp_input);
  }

  glDrawElements (mode, count, type, (void*)offset);
}

void GPUPipeline::push ()
{
  _transform_stack.push_back (_m_transform);
}

void GPUPipeline::pop ()
{
  if (_transform_stack.size () > 0) {
    _m_transform = _transform_stack.back ();
    _transform_stack.pop_back ();
  }
}

void GPUPipeline::set_model_transform (const Math::Matrix4& transform)
{
  _m_transform = transform;
}

void GPUPipeline::set_view_transform (const Math::Matrix4& transform)
{
  _v_transform = transform;
  _vp_dirty = true;
}

void GPUPipeline::set_projection_transform (const Math::Matrix4& transform)
{
  _p_transform = transform;
  _vp_dirty = true;
}

void GPUPipeline::calculate_vp_transform ()
{
  if (!_vp_dirty) return;

  _vp_transform = _v_transform * _p_transform;
  _vp_dirty = false;
}

GPUPipeline::GPUPipeline ()
  : _clear_r (0.0), _clear_g (0.0), _clear_b (0.0), _clear_a (1.0),
    _vp_dirty (true), _shader_mvp_input (nullptr)
{
}

GPUPipeline* GPUPipeline::_instance = nullptr;

GPUPipeline& GPUPipeline::instance ()
{
  if (!_instance)
    _instance = new GPUPipeline ();

  return *_instance;
}

} /* namespace GUI */
