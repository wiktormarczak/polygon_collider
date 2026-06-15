#ifndef BATCH_H
#define BATCH_H

#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>

typedef struct Batch Batch;

Batch *batch_create(unsigned int vertex_buffer_size, unsigned int index_buffer_size);
void batch_destroy(Batch *batch);

void batch_submit_polygon(Batch *batch, Polygon *polygon);
void batch_submit_vector(Batch *batch, Vector position, Vector direction, Color color);

void batch_use(Batch *batch);
void batch_draw_polygons(Batch *batch);
void batch_draw_vectors(Batch *batch);
void batch_flush(Batch *batch);

#endif
