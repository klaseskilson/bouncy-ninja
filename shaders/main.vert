#version 130 compatibility
in vec3 vp;
uniform mat4 MVP;

void main()
{
  gl_Position = MVP * vec4(vp, 1.0);
}
