#version 460
layout (location = 0) in vec2 attribute_position;

void main()
{
    gl_Position = vec4(attribute_position, 0.0, 1.0);
}
