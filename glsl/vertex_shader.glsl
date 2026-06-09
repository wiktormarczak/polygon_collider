#version 460
layout (location = 0) in vec2 attribute_position;
layout (location = 1) in vec3 attribute_color;

out vec3 vertex_color;

uniform mat4 matrix;

void main()
{
    gl_Position = matrix * vec4(attribute_position, 0.0, 1.0);
    vertex_color = attribute_color;
}
