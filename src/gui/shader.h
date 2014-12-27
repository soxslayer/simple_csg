#ifndef __SHADER__
#define __SHADER__

#include "config.h"

#include <string>
#include <list>
#include <stdexcept>

#if defined(HAVE_GL)
# include <GL/gl.h>
#elif defined(HAVE_LOAX)
# include <loax_client_gl2.h>
#endif

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

  class Exception : public CSG::Exception
  {
  public:
    explicit Exception (const std::string& what)
      : CSG::Exception (what) {}
    virtual ~Exception () { }
  };

  class ProgramException : public Exception
  {
  public:
    explicit ProgramException (const std::string& filename,
                               const std::string& reason)
      : Exception (std::string ("Shader exception in ") + filename
                   + std::string (": ") + reason) { }
    virtual ~ProgramException () { }
  };

  Shader (const std::string& vertex_shader_filename,
          const std::string& fragment_shader_filename);
  ~Shader ();

  const Shader::InputDef& get_input (const std::string& name) const;
  GLuint get_program () const { return _program; }

private:
  GLuint _program;
  std::list<InputDef*> _inputs;

  static GLuint load_shader (GLenum type, const std::string& filename);
};

} /* namespace GUI */

#endif /* __SHADER__ */
