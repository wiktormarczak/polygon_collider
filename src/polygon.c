#include <polygon_collider/polygon.h>
#include <polygon_collider/vector.h>
#include <polygon_collider/color.h>
#include <polygon_collider/vertex_buffer.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Polygon
{
    unsigned int vertex_count;
    Vector *vertex, *world_vertex;

    Vector position;
    float orientation;

    Vector linear_velocity;
    float angular_velocity;

    Color color;

    unsigned int vertex_buffer;
};

Polygon *polygon_create(unsigned int vertex_count, Vector *vertex, Color color)
{
    Polygon *polygon = malloc(sizeof(Polygon));

    polygon->vertex_count = vertex_count;

    polygon->vertex = malloc(vertex_count * sizeof(Vector));
    memcpy(polygon->vertex, vertex, vertex_count * sizeof(Vector));
    polygon->world_vertex = malloc(vertex_count * sizeof(Vector));
    memcpy(polygon->world_vertex, vertex, vertex_count * sizeof(Vector));

    polygon->position.x = 0.0f;
    polygon->position.y = 0.0f;
    polygon->orientation = 0.0f;
    polygon->linear_velocity.x = 0.0f;
    polygon->linear_velocity.y = 0.0f;
    polygon->angular_velocity = 0.0f;

    polygon->color = color;
    // polygon->vertex_buffer = vertex_buffer_create(vertex_count);

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

Polygon *polygon_create_regular(unsigned int vertex_count, float radius, Color color)
{
    if(vertex_count < 3)
        return NULL;

    Vector *vertex = malloc(vertex_count * sizeof(Vector));

    int i;
    float delta_angle = (2.0f * 3.1416f) / vertex_count;
    for(i = 0; i < vertex_count; i++)
    {
        vertex[i].x = radius * cos(i * delta_angle);
        vertex[i].y = radius * sin(i * delta_angle);
    }

    Polygon *polygon = polygon_create(vertex_count, vertex, color);
    free(vertex);
    return polygon;
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

void polygon_copy_world_vertex(Polygon *polygon, Vector *destination)
{
    memcpy(destination, polygon->world_vertex, polygon->vertex_count * sizeof(Vector));
}

void polygon_adjust_linear_velocity(Polygon *polygon, Vector delta)
{
    polygon->linear_velocity.x += delta.x;
    polygon->linear_velocity.y += delta.y;
}

void polygon_adjust_angular_velocity(Polygon *polygon, float delta)
{
    polygon->angular_velocity += delta;
}

void polygon_update(Polygon *polygon, float delta_time)
{
    polygon->position.x += delta_time * polygon->linear_velocity.x;
    polygon->position.y += delta_time * polygon->linear_velocity.y;
    polygon->orientation += delta_time * polygon->angular_velocity;

    int i;
    for(i = 0; i < polygon->vertex_count; i++)
    {
        float x = polygon->vertex[i].x;
        float y = polygon->vertex[i].y;
        polygon->world_vertex[i].x = x * cos(polygon->orientation) - y * sin(polygon->orientation) + polygon->position.x;
        polygon->world_vertex[i].y = x * sin(polygon->orientation) + y * cos(polygon->orientation) + polygon->position.y;
    }

    // vertex_buffer_update(polygon->vertex_buffer, polygon->vertex_count, polygon->world_vertex);
}
