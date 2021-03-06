#version 330 core
out vec4 frag_colour;

in vec3 toLight;
in vec3 interpolatedNormal;

void main()
{
  vec3 normal = normalize(interpolatedNormal);
  vec3 toLight2 = normalize(vec3(1.0,2.0,1.0));
  float diffuse = max(0.0, dot(normal, toLight2));
  float ambient = 0.2 ;

  float lightning = diffuse + ambient;

  frag_colour = vec4(0.5, 0.0, 0.0, 1.0) * lightning;
}
