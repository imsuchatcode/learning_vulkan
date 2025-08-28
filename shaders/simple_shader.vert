#version 450

// in mean take value from the vertex buffer | vertex attribute | location mean for refernece in the shader
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

//layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push{
    vec2 offset;
    vec3 color;
} push;

void main() {
    gl_Position = vec4(position + push.offset, 0.0, 1.0);
    //fragColor = color;
}