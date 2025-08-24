#version 450

// in mean take value from the vertex buffer | vertex attribute | location mean for refernece in the shader
layout(location = 0) in vec2 position;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
}