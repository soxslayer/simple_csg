#include <GL/glew.h>
#include <GL/gl.h>

#include "gpu_pipeline.h"

using namespace std;

namespace GUI
{

GPUPipeline::VertexBuffer::VertexBuffer ()
{
  glGenBuffers (1, &_handle);
}

GPUPipeline::VertexBuffer::~VertexBuffer ()
{
  glDeleteBuffers (1, &_handle);
}

void GPUPipeline::VertexBuffer::alloc (const void* data, GLsizeiptr size,
                                       GLenum usage) const
{
  glBindBuffer (GL_ARRAY_BUFFER, _handle);
  glBufferData (GL_ARRAY_BUFFER, size, data, usage);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void GPUPipeline::VertexBuffer::set (const void* data, GLsizeiptr size,
                                     GLintptr offset) const
{
  glBufferSubData (_handle, offset, size, data);
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

void GPUPipeline::set_shader (const Shader& shader)
{
  glUseProgram (shader.get_program ());
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

void GPUPipeline::bind_shader_input (const VertexBuffer& buff, 
                                     const Shader::InputDef& input,
                                     long offset, GLsizei stride) const
{
  if (input.def_type != Shader::InputDef::ATTRIBUTE)
    throw GPUPipelineInputException (input.name, "invalid type");

  glBindBuffer (GL_ARRAY_BUFFER, buff.get_handle ());
  bind_shader_input ((void*)offset, input, stride);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void GPUPipeline::draw (GLenum mode, GLsizei count) const
{
  glDrawArrays (mode, 0, count);
}



GPUPipeline* GPUPipeline::_instance = nullptr;

GPUPipeline& GPUPipeline::instance ()
{
  if (!_instance)
    _instance = new GPUPipeline ();

  return *_instance;
}



GPUPipelineInputException::GPUPipelineInputException (const std::string& input,
  const std::string& reason)
  : CSG::Exception (string ("GPUPipeline input ") + input + string (" ")
                    + reason)
{
}

} /* namespace GUI */
