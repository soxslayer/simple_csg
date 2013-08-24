#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "shader.h"
#include "gpu_pipeline.h"

using namespace std;

int main (int argc, char** argv)
{
  float triangle[] = {
    0.75, 0.75, 0.0, 1.0,
    0.75, -0.75, 0.0, 1.0,
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

  GUI::Shader s ("../gui/vertex.glsl", "../gui/fragment.glsl");

  GUI::GPUPipeline& gpu = GUI::GPUPipeline::instance ();
  gpu.set_shader (s);
  gpu.set_clear_color (0.0, 1.0, 1.0, 0.0);

  GUI::GPUPipeline::VertexBuffer vbo;
  vbo.alloc (triangle, sizeof (triangle));

  const GUI::Shader::InputDef* pos_input = s.get_input ("vertex");
  gpu.bind_shader_input (vbo, *pos_input, 0, 0);

  while (!glfwWindowShouldClose (window)) {
    gpu.clear ();
    gpu.draw (GL_TRIANGLES, 3);
    glfwSwapBuffers (window);

    glfwWaitEvents ();
  }

  glfwTerminate ();

  return 0;
}
