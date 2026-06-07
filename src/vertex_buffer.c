#include <polygon_collider/vertex_buffer.h>
#include <polygon_collider/vector.h>
#include <glad/gl.h>
#include <stddef.h>

unsigned int vertex_buffer_create(unsigned int vertex_count)
{
    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);
    return vertex_buffer;
}

void vertex_buffer_destroy(unsigned int vertex_buffer)
{
    glDeleteBuffers(1, &vertex_buffer);
}

void vertex_buffer_update(unsigned int vertex_buffer, unsigned int vertex_count, Vector *vertex)
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * sizeof(Vector), vertex);
}
