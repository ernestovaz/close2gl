#version 400 core 

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_position;

out vec2 textureCoord;

void main(){
  gl_Position = vec4(vertex_position, 0.0, 1.0);
  textureCoord = texture_position;
}
