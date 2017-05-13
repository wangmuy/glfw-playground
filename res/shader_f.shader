#version 330 core
in vec4 vertexColor;
out vec4 color;
uniform vec4 ourColor;
void main() {
  color = vertexColor; // ourColor; // vertexColor; //vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
