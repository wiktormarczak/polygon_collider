#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <polygon_collider/vector.h>

unsigned int vertex_buffer_create(unsigned int vertex_count);
void vertex_buffer_destroy(unsigned int vertex_buffer);

void vertex_buffer_update(unsigned int vertex_buffer, unsigned int vertex_count, Vector *vertex);

#endif
