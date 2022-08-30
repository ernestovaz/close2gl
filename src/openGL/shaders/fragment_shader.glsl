#version 400 core

out vec4 fragment_color;

in vec4 world_position;
in vec4 world_normal;
in vec4 vertex_interpolated_color;

uniform vec3 color;
uniform vec3 camera_position;

#define DIFFUSE_REFLECTANCE     vec3(1.0, 1.0, 1.0)
#define AMBIENT_REFLECTANCE     vec3(1.0, 1.0, 1.0)
#define SPECULAR_REFLECTANCE    vec3(1.0, 1.0, 1.0)
#define LIGHT_SOURCE_COLOR      vec3(1.0, 1.0, 1.0)
#define AMBIENT_LIGHT_COLOR     vec3(0.1, 0.1, 0.1)

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
  vec4 light_source_direction = normalize(vec4(camera_position, 1.0) + vec4(2.0, 2.0, 2.0, 0.0)- position);
  shaded_color += diffuse_shading(
      DIFFUSE_REFLECTANCE, 
      LIGHT_SOURCE_COLOR, 
      normal, 
      normalize(light_source_direction)
  );
  shaded_color += ambient_shading(
      AMBIENT_REFLECTANCE,
      AMBIENT_LIGHT_COLOR
  );
  vec4 v = normalize(vec4(camera_position, 1.0) - position);
  vec4 r = -light_source_direction + 2 * normal * dot(normal, light_source_direction);
  shaded_color += specular_shading(
      SPECULAR_REFLECTANCE,
      LIGHT_SOURCE_COLOR,
      r,
      v,
      4.0
  );

  return vec4(shaded_color * color, 1.0);
}
