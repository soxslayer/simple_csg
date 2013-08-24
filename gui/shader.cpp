#include <fstream>
#include <string>

#include <GL/glew.h>

#include "shader.h"
#include "csg/exceptions.h"
#include <iostream>

using namespace std;

namespace GUI
{

Shader::Shader (const std::string& vertex_shader_filename,
                const std::string& fragment_shader_filename)
{
  GLuint v_shader = load_shader (GL_VERTEX_SHADER, vertex_shader_filename);
  GLuint f_shader = load_shader (GL_FRAGMENT_SHADER, fragment_shader_filename);

  _program = glCreateProgram ();

  glAttachShader (_program, v_shader);
  glAttachShader (_program, f_shader);

  glLinkProgram (_program);

  GLint status;
  glGetProgramiv (_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint info_len;
    glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &info_len);

    GLchar info_str[info_len + 1];
    glGetProgramInfoLog (_program, info_len, nullptr, info_str);

    throw ShaderProgramException (string ("Shader linker error"));
  }

  glDetachShader (_program, v_shader);
  glDetachShader (_program, f_shader);

  GLint n_attributes;
  glGetProgramiv (_program, GL_ACTIVE_ATTRIBUTES, &n_attributes);

  for (GLint i = 0; i < n_attributes; ++i) {
    InputDef *def = new InputDef ();
    GLsizei name_len;
    char name[100];

    glGetActiveAttrib (_program, i, sizeof (name), &name_len, &def->num,
                       &def->type, name);
    name[name_len] = '\0';

    def->name = name;
    def->def_type = InputDef::ATTRIBUTE;
    def->location = glGetAttribLocation (_program, name);

    _inputs.push_back (def);
  }

  GLint n_uniforms;
  glGetProgramiv (_program, GL_ACTIVE_UNIFORMS, &n_uniforms);

  for (GLint i = 0; i < n_uniforms; ++i) {
    InputDef *def = new InputDef ();
    GLsizei name_len;
    char name[100];

    glGetActiveUniform (_program, i, sizeof (name), &name_len, &def->num,
                        &def->type, name);
    name[name_len] = '\0';

    def->name = name;
    def->def_type = InputDef::UNIFORM;
    def->location = glGetUniformLocation (_program, name);

    _inputs.push_back (def);
  }
}

Shader::~Shader ()
{
  auto d = _inputs.begin ();
  auto d_end = _inputs.end ();

  for (; d != d_end; ++d)
    delete *d;
}

const Shader::InputDef* Shader::get_input (const string& name) const
{
  auto d = _inputs.begin ();
  auto d_end = _inputs.end ();

  for (; d != d_end; ++d) {
    if ((*d)->name == name)
      return *d;
  }

  return nullptr;
}

GLuint Shader::load_shader (GLenum type, const std::string& filename)
{
  GLuint shader = glCreateShader (type);

  ifstream ifs (filename, ios::in);
  if (!ifs.is_open ())
    throw CSG::FileException (filename, "unable to open file");

  ifs.seekg (0, ios_base::end);
  int shader_len = ifs.tellg ();
  ifs.seekg (0, ios_base::beg);
  char shader_str[shader_len + 1];
  ifs.read (shader_str, shader_len);
  shader_str[shader_len] = 0;

  const GLchar *c = shader_str;
  glShaderSource (shader, 1, &c, nullptr);
  glCompileShader (shader);

  GLint status;
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    GLint info_len;
    glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &info_len);

    GLchar info_str[info_len + 1];
    glGetShaderInfoLog (shader, info_len, nullptr, info_str);

    throw ShaderException (filename, string (info_str));
  }

  return shader;
}



ShaderException::ShaderException (const std::string& filename,
                                  const std::string& reason)
  : CSG::Exception (string ("Shader exception in ") + filename + string (": ")
                    + reason)
{
}

ShaderException::~ShaderException ()
{
}



ShaderProgramException::ShaderProgramException (const std::string& reason)
  : CSG::Exception (string ("Shader program exception, ") + reason)
{
}

ShaderProgramException::~ShaderProgramException ()
{
}

} /* namespace GUI */
