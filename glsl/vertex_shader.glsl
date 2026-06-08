#version 460
layout (location = 0) in vec2 attribute_position;

uniform mat4 matrix;

void main()
{
    gl_Position = matrix * vec4(attribute_position, 0.0, 1.0);
}
