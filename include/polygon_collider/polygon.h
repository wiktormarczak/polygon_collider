#ifndef POLYGON_H
#define POLYGON_H

#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>

typedef struct Polygon Polygon;

Polygon *polygon_create(unsigned int vertex_count, Vector *vertex, Color color);
void polygon_destroy(Polygon *polygon);

void polygon_update(Polygon *polygon);

#endif
