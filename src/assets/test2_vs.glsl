#version 410

layout(location = 0) in vec3 vertex_position;

uniform mat4 view, proj;

out vec3 colour;

void main() {
	colour = vec3 (1.0, 1.0, 1.0);
	gl_Position = proj * view * vec4 (vertex_position, 1.0);
}
