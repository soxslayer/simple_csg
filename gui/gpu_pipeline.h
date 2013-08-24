#ifndef __GPU_PIPELINE__
#define __GPU_PIPELINE__

#include <string>

#include "shader.h"
#include "csg/exceptions.h"

namespace GUI
{

class Shader;

class GPUPipeline
{
public:
  class VertexBuffer
  {
  public:
    VertexBuffer ();
    ~VertexBuffer ();

    void alloc (const void* data, GLsizeiptr size,
                GLenum usage = GL_STATIC_DRAW) const;
    void set (const void* data, GLsizeiptr size, GLintptr offset) const;
    GLuint get_handle () const { return _handle; }

  private:
    GLuint _handle;
  };

  void set_clear_color (float r, float g, float b, float a);
  void clear () const;
  void clear (float r, float g, float b, float a) const;
  void set_shader (const Shader& shader);
  void bind_shader_input (void *data, const Shader::InputDef& input,
                          GLsizei stride) const;
  void bind_shader_input (const VertexBuffer& buff, 
                          const Shader::InputDef& input, long offset,
                          GLsizei stride) const;
  void draw (GLenum mode, GLsizei count) const;

  static GPUPipeline& instance ();

private:
  float _clear_r;
  float _clear_g;
  float _clear_b;
  float _clear_a;

  GPUPipeline ();
  GPUPipeline (const GPUPipeline& p) { }
  const GPUPipeline& operator= (const GPUPipeline& p) { return *this; }

  static GPUPipeline* _instance;
};

class GPUPipelineInputException : public CSG::Exception
{
public:
  explicit GPUPipelineInputException (const std::string& input,
                                      const std::string& reason);
  virtual ~GPUPipelineInputException () { }
};

} /* namespace GUI */

#endif /* __GPU_PIPELINE__ */
