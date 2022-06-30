#version 330 core
layout (location = 0) in vec4 vertex_positions;

void main(){
  gl_Position = vertex_positions;
}
