#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "shader.h"
#include "gpu_pipeline.h"
#include "math/matrix4.h"
#include "math/vector3.h"

using namespace std;

int main (int argc, char** argv)
{
  float t1[] = {
    1.0, 1.0, 0.0, 1.0,
    1.0, -1.0, 0.0, 1.0,
    -1.0, 1.0, 0.0, 1.0,
    -1.0, -1.0, 0.0, 1.0
  };

  short e1[] = {
    //0, 1, 2, 1, 2, 3
    0, 1, 1, 2, 2, 0, 1, 2, 2, 3, 3, 1
  };

  if (!glfwInit ()) {
    cerr << "GLFW init failed" << endl;
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow (800, 600, "SimpleCSG", nullptr,
                                         nullptr);
  if (!window) {
    cerr << "GLFW window creation failed" << endl;
    glfwTerminate ();

    return 1;
  }

  glfwMakeContextCurrent (window);

  if (glewInit () != GLEW_OK) {
    cerr << "GLEW init failed" << endl;
    glfwTerminate ();

    return 1;
  }

  GUI::Shader s ("../gui/simple.vert", "../gui/simple.frag");

  GUI::GPUPipeline& gpu = GUI::GPUPipeline::instance ();
  gpu.set_shader (s);

  GUI::GPUPipeline::VertexBuffer vbo1;
  vbo1.alloc (t1, sizeof (t1));

  GUI::GPUPipeline::ElementBuffer ibo1;
  ibo1.alloc (e1, sizeof (e1));

  Math::Matrix4 model1;
  model1.scale (0.5, 0.5, 0.5);

  Math::Matrix4 model2;
  model2.translate (-1.0, 0.0, -5.0);

  Math::Matrix4 view;
  view.look_at (Math::Vector3 (0.0, 0.0, 5.0),
                Math::Vector3 (0.0, 0.0, -1.0),
                Math::Vector3 (0.0, 1.0, 0.0));
  gpu.set_view_transform (view);

  Math::Vector4 color1 (1.0, 0.0, 0.0, 1.0);
  Math::Vector4 color2 (0.0, 1.0, 0.0, 1.0);

  Math::Matrix4 proj;
  proj.ortho_projection (-5, 5, -5, 5, -10, 0);
  gpu.set_projection_transform (proj);

  const GUI::Shader::InputDef* pos_input = s.get_input ("vertex");
  const GUI::Shader::InputDef* color_input = s.get_input ("color");

  while (!glfwWindowShouldClose (window)) {
    gpu.clear ();
    gpu.bind_shader_input (vbo1, *pos_input);
    gpu.bind_shader_input (color1.data (), *color_input);
    gpu.set_model_transform (model1);
    ibo1.bind ();
    //gpu.draw_elements (GL_TRIANGLES, 6, 0);
    gpu.draw_elements (GL_LINES, 12, 0);
    //gpu.draw (GL_TRIANGLES, 6);
    gpu.bind_shader_input (color2.data (), *color_input);
    gpu.set_model_transform (model2);
    //gpu.draw (GL_TRIANGLES, 6);
    glfwSwapBuffers (window);

    glfwWaitEvents ();
  }

  glfwTerminate ();

  return 0;
}
