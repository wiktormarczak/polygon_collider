#ifndef POLYGON_H
#define POLYGON_H

#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>

typedef struct Polygon Polygon;

Polygon *polygon_create(unsigned int vertex_count, Vector *vertex, Color color);
Polygon *polygon_create_regular(unsigned int vertex_count, float radius, Color color);

void polygon_destroy(Polygon *polygon);

unsigned int polygon_get_vertex_count(Polygon *polygon);
Color polygon_get_color(Polygon *polygon);
void polygon_copy_world_vertex(Polygon *polygon, Vector *destination);

void polygon_set_position(Polygon *polygon, Vector position);
void polygon_adjust_linear_velocity(Polygon *polygon, Vector delta);
void polygon_adjust_angular_velocity(Polygon *polygon, float delta);
void polygon_set_color(Polygon *polygon, Color color);

void polygon_update(Polygon *polygon, float delta_time);

#endif
