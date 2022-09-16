#version 400 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texture_coordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 color;
uniform vec3 camera_position;
uniform int use_texture;

//values to be interpolated as fragments values
out vec4 world_position; 
out vec4 world_normal;
out vec4 vertex_interpolated_color;
out vec2 vertex_interpolated_texture_coordinate;

#define DIFFUSE_REFLECTANCE     vec3(1.0, 1.0, 1.0)
#define AMBIENT_REFLECTANCE     vec3(1.0, 1.0, 1.0)
#define SPECULAR_REFLECTANCE    vec3(1.0, 1.0, 1.0)
#define LIGHT_SOURCE_COLOR      vec3(1.0, 1.0, 1.0)
#define AMBIENT_LIGHT_COLOR     vec3(0.1, 0.1, 0.1)
#define LIGHT_SOURCE_DIRECTION  vec4(0.0, 1.0, 0.0, 0.0)

subroutine vec4 shading_mode(vec4 position, vec4 normal, vec3 color);
subroutine uniform shading_mode shading_function;

vec3 diffuse_shading(vec3 kd, vec3 I, vec4 n, vec4 l) {
  return kd * I * max(0, dot(n, l));
}

vec3 ambient_shading(vec3 ka, vec3 Ia) {
  return ka * Ia;
}

vec3 specular_shading(vec3 ks, vec3 I, vec4 r, vec4 v, float q) {
  return ks * I * pow(max(0, dot(r,v)),q);
}


void main(){
  gl_Position = projection * view * model * vec4(vertex_position, 1.0);
  world_position = vec4(vertex_position, 1.0);
  world_normal   = vec4(vertex_normal, 0.0);
  
  vec3 shading_color;
  if(use_texture == 1) shading_color = vec3(1.0);
  else shading_color = color;
  vertex_interpolated_color = shading_function(world_position, world_normal, shading_color);
  vertex_interpolated_texture_coordinate = vertex_texture_coordinate;
}

subroutine(shading_mode) vec4 no_shading(vec4 position, vec4 normal, vec3 color) {
  return vec4(0.0);   // returns black, should not be used in fragment shader
}

subroutine(shading_mode) vec4 ambient_diffuse_shading(vec4 position, vec4 normal, vec3 color) {
  vec3 shaded_color = vec3(0.0);
  vec4 light_source_direction = normalize(vec4(camera_position, 1.0) + vec4(2.0, 2.0, 2.0, 0.0) - position);
  shaded_color += diffuse_shading(
      DIFFUSE_REFLECTANCE, 
      LIGHT_SOURCE_COLOR, 
      normal, 
      light_source_direction
  );
  shaded_color += ambient_shading(
      AMBIENT_REFLECTANCE,
      AMBIENT_LIGHT_COLOR
  );
  return vec4(shaded_color * color, 1.0);
}

subroutine(shading_mode) vec4 ambient_diffuse_specular_shading(vec4 position, vec4 normal, vec3 color) {
  vec3 shaded_color = vec3(0.0);
  vec4 light_source_direction = normalize(vec4(camera_position, 1.0) + vec4(2.0, 2.0, 2.0, 0.0) - position);
  shaded_color += diffuse_shading(
      DIFFUSE_REFLECTANCE, 
      LIGHT_SOURCE_COLOR, 
      normal, 
      light_source_direction
  );
  shaded_color += ambient_shading(
      AMBIENT_REFLECTANCE,
      AMBIENT_LIGHT_COLOR
  );
  vec4 v = normalize(vec4(camera_position,1.0) - position);
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
