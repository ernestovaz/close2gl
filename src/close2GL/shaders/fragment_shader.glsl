#version 400 core

out vec4 fragment_color;

in vec2 textureCoord;
uniform sampler2D textureInput;

void main() {
  fragment_color = texture(textureInput, textureCoord);
}
