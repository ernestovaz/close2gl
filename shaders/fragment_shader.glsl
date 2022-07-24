#version 450 core
out vec4 fragment_color;

in vec4 world_fragment_position;
in vec4 world_fragment_normal;

subroutine vec3 shading_mode();
subroutine uniform shading_mode shading_function;

uniform vec3 color;

subroutine(shading_mode) vec3 no_shading() {
  return color;
}

void main() {
  fragment_color = vec4(shading_function(), 1.0);
}

