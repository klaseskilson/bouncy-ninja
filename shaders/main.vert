#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 toLight;
out vec3 interpolatedNormal;

uniform mat4 MVP;

void main()
{
  interpolatedNormal = normal;
  vec3 lightPos = vec3(-3.0, -3.0, 3.0);
  toLight = normalize(lightPos - position);
  gl_Position = MVP * vec4(position, 1.0);
}
