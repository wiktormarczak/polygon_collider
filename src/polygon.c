#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>
#include <polygon_collider/vertex_buffer.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Polygon
{
    unsigned int vertex_count;
    Vector *vertex;
    Color color;

    unsigned int vertex_buffer;
};

Polygon *polygon_create(unsigned int vertex_count, Vector *vertex, Color color)
{
    Polygon *polygon = malloc(sizeof(Polygon));

    polygon->vertex_count = vertex_count;
    memcpy(polygon->vertex, vertex, vertex_count * sizeof(Vector));
    polygon->color = color;
    polygon->vertex_buffer = vertex_buffer_create(vertex_count);

    return polygon;
}

void polygon_destroy(Polygon *polygon)
{
    free(polygon->vertex);
    polygon->vertex = NULL;

    free(polygon);
}

void polygon_update(Polygon *polygon)
{
    vertex_buffer_update(polygon->vertex_buffer, polygon->vertex_count, polygon->vertex);
}
