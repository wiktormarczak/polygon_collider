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
    polygon->vertex = malloc(vertex_count * sizeof(Vector));
    memcpy(polygon->vertex, vertex, vertex_count * sizeof(Vector));
    polygon->color = color;
    polygon->vertex_buffer = vertex_buffer_create(vertex_count);

    return polygon;
}

Polygon *polygon_create_triangle(Color color)
{
    Vector vertex[3];
    vertex[0].x = 0.0f;
    vertex[0].y = 0.5f;
    vertex[1].x = -0.5f;
    vertex[1].y = -0.5f;
    vertex[2].x = 0.5f;
    vertex[2].y = -0.5f;

    return polygon_create(3, vertex, color);
}

void polygon_destroy(Polygon *polygon)
{
    free(polygon->vertex);
    polygon->vertex = NULL;

    free(polygon);
}

unsigned int polygon_get_vertex_count(Polygon *polygon)
{
    return polygon->vertex_count;
}

Color polygon_get_color(Polygon *polygon)
{
    return polygon->color;
}

unsigned int polygon_get_vertex_buffer(Polygon *polygon)
{
    return polygon->vertex_buffer;
}

void polygon_update(Polygon *polygon)
{
    vertex_buffer_update(polygon->vertex_buffer, polygon->vertex_count, polygon->vertex);
}
