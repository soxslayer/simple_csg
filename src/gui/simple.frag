#ifdef GL_ES

precision mediump float;
precision mediump int;

#endif

varying vec4 f_color;

void main ()
{
  gl_FragColor = f_color;
}
