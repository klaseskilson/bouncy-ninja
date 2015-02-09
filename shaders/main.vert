#version 130 compatibility
in vec3 vp;

void main()
{
  gl_Position = vec4(vp, 1.0);
}
