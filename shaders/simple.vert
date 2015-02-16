#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 toLight;
out vec3 interpolatedNormal;

uniform mat4 MVP;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
  interpolatedNormal = normal;
  vec3 lightPos = vec3(-3.0, -3.0, 3.0);
  toLight = normalize(lightPos - position);
  gl_Position = (P * V * M) * vec4(position, 1.0);
}
