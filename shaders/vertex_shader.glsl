#version 450 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//values to be interpolated as fragments values
out vec4 world_fragment_position; 
out vec4 world_fragment_normal;

void main(){
  gl_PointSize = 5.0;
  gl_Position = projection * view * model * vec4(vertex_position, 1.0);
  
  world_fragment_position = model * vec4(vertex_position, 1.0);

  // normals should be translated inversely
  world_fragment_normal   = inverse(transpose(model)) * vec4(vertex_normal, 0.0);
  world_fragment_normal.w = 0.0; // w is discarded, vectors should have w = 0
}
