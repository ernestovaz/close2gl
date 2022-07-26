#version 400 core

out vec4 fragment_color;

in vec4 world_position;
in vec4 world_normal;
in vec4 vertex_interpolated_color;
in vec4 camera_position;

uniform vec3 color;

#define DIFFUSE_REFLECTANCE     vec3(0.9, 0.9, 0.9)
#define AMBIENT_REFLECTANCE     vec3(0.3, 0.3, 0.3)
#define SPECULAR_REFLECTANCE    vec3(1.5, 1.5, 1.5)
#define LIGHT_SOURCE_COLOR      vec3(1.0, 1.0, 1.0)
#define AMBIENT_LIGHT_COLOR     vec3(0.1, 0.1, 0.1)
#define LIGHT_SOURCE_DIRECTION  vec4(0.0, 1.0, 0.0, 0.0)

vec3 diffuse_shading(vec3 kd, vec3 I, vec4 n, vec4 l) {
  return kd * I * max(0, dot(n, l));
}

vec3 ambient_shading(vec3 ka, vec3 Ia) {
  return ka * Ia;
}

vec3 specular_shading(vec3 ks, vec3 I, vec4 r, vec4 v, float q) {
  return ks * I * pow(max(0, dot(r,v)),q);
}

subroutine vec4 shading_mode(vec4 position, vec4 normal);
subroutine uniform shading_mode shading_function;


void main() {
  fragment_color = shading_function(world_position, normalize(world_normal));
}


subroutine(shading_mode) vec4 no_shading(vec4 position, vec4 normal) {
  return vec4(color, 1.0);
}

subroutine(shading_mode) vec4 use_vertex_color(vec4 position, vec4 normal) {
  return vertex_interpolated_color;
}

subroutine(shading_mode) vec4 ambient_diffuse_specular_shading(vec4 position, vec4 normal) {
  vec3 shaded_color = vec3(0.0);
  shaded_color += diffuse_shading(
      DIFFUSE_REFLECTANCE, 
      LIGHT_SOURCE_COLOR, 
      normal, 
      LIGHT_SOURCE_DIRECTION
  );
  shaded_color += ambient_shading(
      AMBIENT_REFLECTANCE,
      AMBIENT_LIGHT_COLOR
  );
  vec4 v = normalize(camera_position - position);
  vec4 r = -LIGHT_SOURCE_DIRECTION + 2 * normal * dot(normal, LIGHT_SOURCE_DIRECTION);
  shaded_color += specular_shading(
      SPECULAR_REFLECTANCE,
      LIGHT_SOURCE_COLOR,
      r,
      v,
      40.0
  );

  return vec4(shaded_color * color, 1.0);
}
