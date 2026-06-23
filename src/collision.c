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
#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/edge.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
    unsigned int vertex_count;
    Vector *vertex;
} CollisionBox;

typedef enum
{
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
} Direction;

static CollisionBox *collision_box_create(Polygon *polygon);
static void collision_box_destroy(CollisionBox *collision_box);
static float collision_get_min_overlap(CollisionBox *left, CollisionBox *right, Vector *contact_point_destination, Vector *axis_destination);
static float collision_get_projection_min(Vector axis, Edge edge, CollisionBox *collision_box, Vector *contact_point_destination);

bool collision_handle(Polygon *left, Polygon *right, VectorObjectQueue *vector_object_queue)
{
    Vector contact_point, axis;
    if(!collision_check(left, right, &contact_point, &axis))
        return false;

    polygon_translate(left, axis);
    axis = vector_get_normalized(axis);

    float a_left, b_left, a_right, b_right;
    polygon_copy_collision_parameters(left, contact_point, axis, &a_left, &b_left);
    polygon_copy_collision_parameters(right, contact_point, vector_get_negated(axis), &a_right, &b_right);

    float a = a_left + a_right;
    float b = b_left + b_right;

    float impulse = -b / a;
    polygon_apply_impulse(left, contact_point, vector_get_scaled(axis, impulse));
    polygon_apply_impulse(right, contact_point, vector_get_scaled(axis, -impulse));

    VectorObject *vector_object_left = vector_object_create();
    vector_object_set_vector(vector_object_left, axis);
    vector_object_set_position(vector_object_left, contact_point);
    vector_object_set_color(vector_object_left, polygon_get_color(right));
    vector_object_queue_submit_vector(vector_object_queue, vector_object_left);
    vector_object_left = NULL;

    VectorObject *vector_object_right = vector_object_create();
    vector_object_set_vector(vector_object_right, vector_get_negated(axis));
    vector_object_set_position(vector_object_right, contact_point);
    vector_object_set_color(vector_object_right, polygon_get_color(left));
    vector_object_queue_submit_vector(vector_object_queue, vector_object_right);
    vector_object_right = NULL;

    return true;
}

void collision_handle_with_wall(Polygon *polygon, VectorObjectQueue *vector_object_queue)
{
    const float bound[] = { -6.67f, 6.67f, -5.0f, 5.0f };
    const Vector axis[] = { vector_get(1.0f, 0.0f), vector_get(-1.0f, 0.0f), vector_get(0.0f, 1.0f), vector_get(0.0f, -1.0f) };
    const Edge edge[] = {
        edge_get(vector_get(bound[LEFT], bound[BOTTOM]), vector_get(bound[LEFT], bound[TOP])),
        edge_get(vector_get(bound[RIGHT], bound[BOTTOM]), vector_get(bound[RIGHT], bound[TOP])),
        edge_get(vector_get(bound[LEFT], bound[BOTTOM]), vector_get(bound[RIGHT], bound[BOTTOM])),
        edge_get(vector_get(bound[LEFT], bound[TOP]), vector_get(bound[RIGHT], bound[TOP]))
    };

    CollisionBox *collision_box = collision_box_create(polygon);

    for(int i = 0; i < 4; i++)
    {
        Vector contact_point;
        float polygon_value = collision_get_projection_min(axis[i], edge[i], collision_box, &contact_point);
        float edge_value = -fabs(bound[i]);

        if(polygon_value < edge_value)
        {
            polygon_translate(polygon, vector_get_scaled(axis[i], edge_value - polygon_value));

            float a, b;
            polygon_copy_collision_parameters(polygon, contact_point, axis[i], &a, &b);

            float impulse = -b / a;
            polygon_apply_impulse(polygon, contact_point, vector_get_scaled(axis[i], impulse));

            VectorObject *vector_object = vector_object_create();
            vector_object_set_vector(vector_object, axis[i]);
            vector_object_set_position(vector_object, contact_point);
            vector_object_set_color(vector_object, color_get(1.0f, 1.0f, 1.0f));
            vector_object_queue_submit_vector(vector_object_queue, vector_object);
            vector_object = NULL;
        }
    }
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
    Vector axis = vector_get_scaled(axis_left, overlap_left);

    if(overlap_right < overlap)
    {
        overlap = overlap_right;
        contact_point = contact_point_right;
        axis = vector_get_scaled(axis_right, -overlap_right);
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
        float right_min = collision_get_projection_min(axis, edge_get(left->vertex[i], left->vertex[(i + 1) % left->vertex_count]), right, &contact_point);

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

static float collision_get_projection_min(Vector axis, Edge edge, CollisionBox *collision_box, Vector *contact_point_destination)
{
    float min = FLT_MAX;
    Vector contact_point_1, contact_point_2;
    bool db = false;

    for(int i = 0; i < collision_box->vertex_count; i++)
    {
        float value = vector_get_dot_product(axis, collision_box->vertex[i]);

        if(fabs(value - min) < 0.01f)
        {
            contact_point_2 = collision_box->vertex[i];
            db = true;
            continue;
        }

        if(value < min)
        {
            min = value;
            contact_point_1 = collision_box->vertex[i];
            db = false;
            continue;
        }
    }

    Vector contact_point = contact_point_1;

    if(db)
    {
        Vector point[4];
        point[0] = edge.initial_point;
        point[1] = edge.terminal_point;
        point[2] = contact_point_1;
        point[3] = contact_point_2;

        for(int i = 0; i < 4; i++)
        {
            float min_v = FLT_MAX;
            unsigned int min_j = -1;

            for(int j = i; j < 4; j++)
            {
                float v = vector_get_cross_product(axis, point[j]);

                if(v < min_v)
                {
                    min_v = v;
                    min_j = j;
                }
            }

            Vector buffer = point[i];
            point[i] = point[min_j];
            point[min_j] = buffer;
        }

        contact_point = vector_get_average(point[1], point[2]);
    }

    *contact_point_destination = contact_point;
    return min;
}
