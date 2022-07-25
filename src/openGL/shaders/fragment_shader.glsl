#version 330 core
#extension GL_ARB_shader_subroutine : require
out vec4 fragment_color;

in vec4 world_position;
in vec4 world_normal;
in vec4 vertex_interpolated_color;

subroutine vec4 shading_mode();
subroutine uniform shading_mode shading_function;

uniform vec3 color;

subroutine(shading_mode) vec4 no_shading() {
  return vec4(color, 1.0);
}

subroutine(shading_mode) vec4 use_vertex_color() {
  return vertex_interpolated_color;
}

void main() {
  fragment_color = shading_function();
}

