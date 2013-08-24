#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "shader.h"
#include "gpu_pipeline.h"

using namespace std;

int main (int argc, char** argv)
{
  float t1[] = {
    0.75, 0.75, 0.0, 1.0,
    0.75, -0.75, 0.0, 1.0,
    -0.75, -0.75, 0.0, 1.0
  };

  float t2[] = {
    -0.75, 0.75, 0.0, 1.0,
    0.75, 0.75, 0.0, 1.0,
    -0.75, -0.75, 0.0, 1.0
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
  gpu.set_clear_color (0.0, 1.0, 1.0, 0.0);

  GUI::GPUPipeline::VertexBuffer vbo1;
  vbo1.alloc (t1, sizeof (t1));

  GUI::GPUPipeline::VertexBuffer vbo2;
  vbo2.alloc (t2, sizeof (t2));

  const GUI::Shader::InputDef* pos_input = s.get_input ("vertex");

  while (!glfwWindowShouldClose (window)) {
    gpu.clear ();
    gpu.bind_shader_input (vbo1, *pos_input, 0, 0);
    gpu.draw (GL_TRIANGLES, 3);
    gpu.bind_shader_input (vbo2, *pos_input, 0, 0);
    gpu.draw (GL_TRIANGLES, 3);
    glfwSwapBuffers (window);

    glfwWaitEvents ();
  }

  glfwTerminate ();

  return 0;
}
