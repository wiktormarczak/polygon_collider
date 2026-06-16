/* Copyright 2026 Wiktor Marczak 
 * This file is part of Polygon Collider. */

/* Polygon Collider is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. */

/* Polygon Collider is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License
 * along with Polygon Collider. If not, see <https://www.gnu.org/licenses/>. */

#include <polygon_collider/collision.h>
#include <polygon_collider/vector.h>
#include <float.h>
#include <stdlib.h>

typedef struct
{
    unsigned int vertex_count;
    Vector *vertex;
} CollisionBox;

static CollisionBox *collision_box_create(Polygon *polygon);
static void collision_box_destroy(CollisionBox *collision_box);
static float collision_get_min_overlap(CollisionBox *left, CollisionBox *right, Vector *contact_point_destination, Vector *axis_destination);
static float collision_get_projection_min(Vector axis, CollisionBox *collision_box, Vector *contact_point_destination);

void collision_handle(Polygon *left, Polygon *right)
{
}

bool collision_check(Polygon *left, Polygon *right, Vector *contact_point_destination, Vector *axis_destination)
{
    CollisionBox *collision_box_left = collision_box_create(left);
    CollisionBox *collision_box_right = collision_box_create(right);

    Vector contact_point_left, axis_left, contact_point_right, axis_right;
    float overlap_left = collision_get_min_overlap(collision_box_left, collision_box_right, &contact_point_left, &axis_left);
    float overlap_right = collision_get_min_overlap(collision_box_right, collision_box_left, &contact_point_right, &axis_right);

    float overlap = overlap_left;
    Vector contact_point = contact_point_left;
    Vector axis = axis_left;

    if(overlap_right < overlap)
    {
        overlap = overlap_right;
        contact_point = contact_point_right;
        axis = vector_get_negated(axis_right);
    }

    if(overlap <= 0.0)
        return false;

    *contact_point_destination = contact_point;
    *axis_destination = axis;
    return true;
}

bool collision_is_point_inside(Vector point, unsigned int vertex_count, Vector *vertex)
{
    for(int i = 0; i < vertex_count; i++)
    {
        Vector axis = vector_get_normal(vertex[i], vertex[(i + 1) % vertex_count]);
        float point_value = vector_get_dot_product(axis, point);
        float polygon_value = vector_get_dot_product(axis, vertex[i]);

        if(point_value > polygon_value)
            return false;
    }

    return true;
}

static CollisionBox *collision_box_create(Polygon *polygon)
{
    CollisionBox *collision_box = malloc(sizeof(CollisionBox));

    collision_box->vertex_count = polygon_get_vertex_count(polygon);
    collision_box->vertex = malloc(collision_box->vertex_count * sizeof(Vector));
    polygon_copy_world_vertex(polygon, collision_box->vertex);

    return collision_box;
}

static void collision_box_destroy(CollisionBox *collision_box)
{
    collision_box->vertex_count = 0;

    free(collision_box->vertex);
    collision_box->vertex = NULL;

    free(collision_box);
    collision_box = NULL;
}

static float collision_get_min_overlap(CollisionBox *left, CollisionBox *right, Vector *contact_point_destination, Vector *axis_destination)
{
    float min_overlap = FLT_MAX;
    Vector min_contact_point, min_axis;

    for(int i = 0; i < left->vertex_count; i++)
    {
        Vector axis = vector_get_normal(left->vertex[i], left->vertex[(i + 1) % left->vertex_count]);

        Vector contact_point;
        float left_max = vector_get_dot_product(axis, left->vertex[i]);
        float right_min = collision_get_projection_min(axis, right, &contact_point);

        float overlap = left_max - right_min;

        if(overlap < min_overlap)
        {
            min_overlap = overlap;
            min_contact_point = contact_point;
            min_axis = axis;
        }
    }

    *contact_point_destination = min_contact_point;
    *axis_destination = vector_get_negated(min_axis);
    return min_overlap;
}

static float collision_get_projection_min(Vector axis, CollisionBox *collision_box, Vector *contact_point_destination)
{
    float min = FLT_MAX;
    Vector contact_point;

    for(int i = 0; i < collision_box->vertex_count; i++)
    {
        float value = vector_get_dot_product(axis, collision_box->vertex[i]);

        if(value < min)
        {
            min = value;
            contact_point = collision_box->vertex[i];
        }
    }

    *contact_point_destination = contact_point;
    return min;
}
