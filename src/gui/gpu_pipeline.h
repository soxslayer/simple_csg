#ifndef __GPU_PIPELINE__
#define __GPU_PIPELINE__

#include <string>
#include <list>
#include <stdexcept>
#include <utility>

#include "shader.h"
#include "csg/exceptions.h"
#include "math/matrix.h"

namespace GUI
{

class Shader;

class GPUPipeline
{
public:
  class Buffer
  {
  private:
    struct Handle
    {
      Handle();
      ~Handle();

      GLuint handle;
    };

  public:
    Buffer(GLenum type);
    Buffer(Buffer&& buff);
    virtual ~Buffer();

    const Buffer& operator=(Buffer&& buff);

    void alloc(const void* data, GLsizei count, GLsizeiptr item_size,
               GLenum usage = GL_STATIC_DRAW);
    void set(const void* data, GLsizei count, GLsizeiptr item_size,
             GLintptr offset);
    void bind();
    void unbind() const;
    GLuint get_handle() const { return _handle->handle; }
    GLenum get_type() const { return _type; }
    GLsizei get_count() const { return _count; }

  private:
    Handle* _handle;
    GLenum _type;
    GLsizei _count;
  };

  class VertexBuffer : public Buffer
  {
  public:
    VertexBuffer() : Buffer(GL_ARRAY_BUFFER) { }
    VertexBuffer(VertexBuffer&& buff) : Buffer(std::move(buff)) { }
    VertexBuffer(const float* buff, GLsizei count);

    const VertexBuffer& operator=(VertexBuffer&& buff);

    void alloc(const float* buff, GLsizei count);
    void set(const float* buff, GLsizei count, GLintptr offset);
  };

  class ElementBuffer : public Buffer
  {
  public:
    ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) { }
    ElementBuffer(ElementBuffer&& buff) : Buffer(std::move(buff)) { }
    ElementBuffer(const short* buff, GLsizei count);

    const ElementBuffer& operator=(ElementBuffer&& buff);

    void alloc(const short* buff, GLsizei count);
    void set(const short* buff, GLsizei count, GLintptr offset);
  };

  class Exception : public CSG::Exception
  {
  public:
    explicit Exception(const std::string& what)
      : CSG::Exception(what) { }
  };

  class InputException : public Exception
  {
  public:
    explicit InputException(const std::string& input,
                            const std::string& reason)
      : Exception(std::string("GPUPipeline input ") + input
                  + std::string(" ") + reason) { }
    virtual ~InputException() { }
  };

  void set_clear_color(float r, float g, float b, float a = 1.0);
  void clear() const;
  void clear(float r, float g, float b, float a) const;
  const Shader& get_shader() const;
  void set_shader(const Shader& shader);
  void bind_shader_input(void* data, const Shader::InputDef& input,
                         GLsizei stride = 0);
  void bind_shader_input(void* data, const std::string& input,
                         GLsizei stride = 0);
  void bind_shader_input(Buffer& buff,
                         const Shader::InputDef& input, long offset = 0,
                         GLsizei stride = 0);
  void bind_shader_input(Buffer& buff,
                         const std::string& input, long offset = 0,
                         GLsizei stride = 0);
  void draw(GLenum mode, GLsizei count);
  void draw_elements(GLenum mode, GLsizei count, long offset,
                     GLenum type = GL_UNSIGNED_SHORT);

  void push();
  void pop();
  void set_model_transform(const Math::Matrix4<float>& transform);
  void reset_model_transform() { _m_transform.make_identity(); }
  const Math::Matrix4<float>& get_model_transform() const
  { return _m_transform; }
  void set_view_transform(const Math::Matrix4<float>& transform);
  void reset_view_transform()
  { _v_transform.make_identity(); _vp_dirty = true; }
  const Math::Matrix4<float>& get_view_transform() const
  { return _v_transform; }
  void set_projection_transform(const Math::Matrix4<float>& transform);
  void reset_projection_transform()
  { _p_transform.make_identity(); _vp_dirty = true; }
  const Math::Matrix4<float>& get_projection_transform() const
  { return _p_transform; }
  void set_viewport(float x_dim, float y_dim) const;

  void enable_face_culling() const;
  void disable_face_culling() const;
  void enable_depth_buffer() const;
  void disable_depth_buffer() const;

  static GPUPipeline& instance();

private:
  float _clear_r;
  float _clear_g;
  float _clear_b;
  float _clear_a;
  std::list<Math::Matrix4<float>> _transform_stack;
  Math::Matrix4<float> _m_transform;
  Math::Matrix4<float> _v_transform;
  Math::Matrix4<float> _p_transform;
  Math::Matrix4<float> _vp_transform;
  bool _vp_dirty;
  const Shader::InputDef* _shader_mvp_input;
  const Shader* _shader;

  GPUPipeline();
  GPUPipeline(const GPUPipeline& p) { }
  const GPUPipeline& operator=(const GPUPipeline& p) { return *this; }

  void calculate_vp_transform();

  static GPUPipeline* _instance;

  int get_type_size(GLenum type);
};

} /* namespace GUI */

#endif /* __GPU_PIPELINE__ */
