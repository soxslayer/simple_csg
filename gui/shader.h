#ifndef __SHADER__
#define __SHADER__

#include <string>
#include <list>

#include <GL/gl.h>

#include "csg/exceptions.h"

namespace GUI
{

class Shader
{
public:
  struct InputDef
  {
    typedef enum
    {
      ATTRIBUTE,
      UNIFORM
    } Type;

    Type def_type;
    std::string name;
    GLenum type;
    GLint num;
    GLuint location;

    friend class Shader;

  private:
    InputDef () { }
    InputDef (const InputDef& d) { }
    const InputDef& operator= (const InputDef& d) { return *this; }
  };

  Shader (const std::string& vertex_shader_filename,
          const std::string& fragment_shader_filename);
  ~Shader ();

  const Shader::InputDef* get_input (const std::string& name) const;
  GLuint get_program () const { return _program; }

private:
  GLuint _program;
  std::list<InputDef*> _inputs;

  static GLuint load_shader (GLenum type, const std::string& filename);
};

class ShaderException : public CSG::Exception
{
public:
  explicit ShaderException (const std::string& filename,
                            const std::string& reason);
  virtual ~ShaderException ();
};

class ShaderProgramException : public CSG::Exception
{
public:
  explicit ShaderProgramException (const std::string& reason);
  virtual ~ShaderProgramException ();
};

} /* namespace GUI */

#endif /* __SHADER__ */
