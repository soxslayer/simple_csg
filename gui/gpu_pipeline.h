#ifndef __GPU_PIPELINE__
#define __GPU_PIPELINE__

#include <string>
#include <list>

#include "shader.h"
#include "csg/exceptions.h"
#include "math/matrix4.h"

namespace GUI
{

class Shader;

class GPUPipeline
{
public:
  class Buffer
  {
  public:
    Buffer (GLenum type);
    virtual ~Buffer ();

    void alloc (const void* data, GLsizeiptr size,
                GLenum usage = GL_STATIC_DRAW) const;
    void set (const void* data, GLsizeiptr size, GLintptr offset) const;
    void bind () const;
    void unbind () const;
    GLuint get_handle () const { return _handle; }

  private:
    GLenum _type;
    GLuint _handle;
  };

  class VertexBuffer : public Buffer
  {
  public:
    VertexBuffer () : Buffer (GL_ARRAY_BUFFER) { }
  };

  class ElementBuffer : public Buffer
  {
  public:
    ElementBuffer () : Buffer (GL_ELEMENT_ARRAY_BUFFER) { }
  };

  void set_clear_color (float r, float g, float b, float a);
  void clear () const;
  void clear (float r, float g, float b, float a) const;
  void set_shader (const Shader& shader);
  void bind_shader_input (void *data, const Shader::InputDef& input,
                          GLsizei stride = 0) const;
  void bind_shader_input (const Buffer& buff,
                          const Shader::InputDef& input, long offset = 0,
                          GLsizei stride = 0) const;
  void draw (GLenum mode, GLsizei count);
  void draw_elements (GLenum mode, GLsizei count, long offset,
                      GLenum type = GL_UNSIGNED_SHORT);

  void push ();
  void pop ();
  void set_model_transform (const Math::Matrix4& transform);
  void reset_model_transform () { _m_transform.reset (); }
  const Math::Matrix4& get_model_transform () const
  { return _m_transform; }
  void set_view_transform (const Math::Matrix4& transform);
  void reset_view_transform () { _v_transform.reset (); _vp_dirty = true; }
  const Math::Matrix4& get_view_transform () const { return _v_transform; }
  void set_projection_transform (const Math::Matrix4& transform);
  void reset_projection_transform ()
  { _p_transform.reset (); _vp_dirty = true; }
  const Math::Matrix4& get_projection_transform () const
  { return _p_transform; }

  static GPUPipeline& instance ();

private:
  float _clear_r;
  float _clear_g;
  float _clear_b;
  float _clear_a;
  std::list<Math::Matrix4> _transform_stack;
  Math::Matrix4 _m_transform;
  Math::Matrix4 _v_transform;
  Math::Matrix4 _p_transform;
  Math::Matrix4 _vp_transform;
  bool _vp_dirty;
  const Shader::InputDef* _shader_mvp_input;

  GPUPipeline ();
  GPUPipeline (const GPUPipeline& p) { }
  const GPUPipeline& operator= (const GPUPipeline& p) { return *this; }

  void calculate_vp_transform ();

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
