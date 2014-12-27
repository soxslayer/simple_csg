attribute vec4 vertex;
uniform vec4 color;
uniform mat4 mvp_transform;

varying vec4 f_color;

void main ()
{
  gl_Position = mvp_transform * vertex;
  f_color = color;
}
