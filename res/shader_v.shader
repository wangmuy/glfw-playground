#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
out vec4 vertexColor;
void main() {
  gl_Position = vec4(position, 1.0);
vertexColor = vec4(color, 1.0); //vec4(0.5f, 0.0f, 0.0f, 1.0f);
}
