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

static void collision_project_on_axis(Vector axis, unsigned int vertex_count, Vector *vertex, float *min, float *max)
{
    *min = FLT_MAX;
    *max = FLT_MIN;

    for(int i = 0; i < vertex_count; i++)
    {
        float value = vector_get_dot_product(axis, vertex[i]);

        if(value < *min)
            *min = value;

        if(value > *max)
            *max = value;
    }
}

static bool collision_check_axis(Vector axis, unsigned int left_vertex_count, Vector *left_vertex, unsigned int right_vertex_count, Vector *right_vertex)
{
    float left_min, left_max;
    collision_project_on_axis(axis, left_vertex_count, left_vertex, &left_min, &left_max);

    float right_min, right_max;
    collision_project_on_axis(axis, right_vertex_count, right_vertex, &right_min, &right_max);

    if(left_min > right_max || left_max < right_min)
        return false;
    return true;
}

bool collision_check(Polygon *left, Polygon *right)
{
    unsigned int left_vertex_count = polygon_get_vertex_count(left);
    Vector *left_vertex = malloc(left_vertex_count * sizeof(Vector));
    polygon_copy_world_vertex(left, left_vertex);

    unsigned int right_vertex_count = polygon_get_vertex_count(right);
    Vector *right_vertex = malloc(right_vertex_count * sizeof(Vector));
    polygon_copy_world_vertex(right, right_vertex);

    bool result = true;

    for(int i = 0; i < left_vertex_count; i++)
    {
        Vector axis = vector_get_normal(left_vertex[i], left_vertex[(i + 1) % left_vertex_count]);

        if(collision_check_axis(axis, left_vertex_count, left_vertex, right_vertex_count, right_vertex) == false)
        {
            result = false;
            break;
        }
    }

    if(result == false)
    {
        free(left_vertex);
        free(right_vertex);
        return result;
    }

    for(int i = 0; i < right_vertex_count; i++)
    {
        Vector axis = vector_get_normal(right_vertex[i], right_vertex[(i + 1) % right_vertex_count]);

        if(collision_check_axis(axis, left_vertex_count, left_vertex, right_vertex_count, right_vertex) == false)
        {
            result = false;
            break;
        }
    }

    free(left_vertex);
    free(right_vertex);
    return result;
}
