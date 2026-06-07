#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Polygon
{
    int vertex_count;
    Vector *vertex;
    Color color;
};

Polygon *polygon_create(int vertex_count, Vector *vertex, Color color)
{
    Polygon *polygon = malloc(sizeof(Polygon));

    polygon->vertex_count = vertex_count;
    memcpy(polygon->vertex, vertex, vertex_count * sizeof(Vector));
    polygon->color = color;

    return polygon;
}

void polygon_destroy(Polygon *polygon)
{
    free(polygon->vertex);
    polygon->vertex = NULL;

    free(polygon);
    polygon = NULL;
}
