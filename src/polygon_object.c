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

#include <polygon_collider/polygon_object.h>
#include <polygon_collider/geometry/polygon.h>
#include <polygon_collider/geometry/vector.h>
#include <polygon_collider/collision.h>
#include <polygon_collider/color.h>

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct PolygonObject
{
    Polygon *local, *global;

    Vector position;
    double orientation;

    double linear_mass;
    double angular_mass;

    Vector linear_velocity;
    double angular_velocity;

    Color color;
};

typedef enum
{
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
} Direction;

PolygonObject *polygon_object_create_regular(unsigned int vertex_count, double radius, Color color)
{
    PolygonObject *polygon_object = malloc(sizeof(PolygonObject));

    polygon_object->local = polygon_create_regular(vertex_count, radius);
    polygon_object->global = polygon_create_regular(vertex_count, radius);

    polygon_object->position.x = 0.0f;
    polygon_object->position.y = 0.0f;
    polygon_object->orientation = 0.0f;

    polygon_object->linear_mass = polygon_get_linear_mass(polygon_object->local);
    polygon_object->angular_mass = polygon_get_angular_mass(polygon_object->local);

    polygon_object->linear_velocity.x = 0.0f;
    polygon_object->linear_velocity.y = 0.0f;
    polygon_object->angular_velocity = 0.0f;

    polygon_object->color = color;

    printf("Angular mass: %f\n", polygon_object->angular_mass);

    return polygon_object;
}

void polygon_object_destroy(PolygonObject *polygon_object)
{
    // free(polygon_object->vertex);
    // polygon_object->vertex = NULL;

    free(polygon_object);
}

unsigned int polygon_object_get_vertex_count(PolygonObject *polygon_object)
{
    return polygon_get_vertex_count(polygon_object->local);
}

Color polygon_object_get_color(PolygonObject *polygon_object)
{
    return polygon_object->color;
}

double polygon_object_get_energy(PolygonObject *polygon_object)
{
    double speed = vector_get_length(polygon_object->linear_velocity);
    return 0.5 * polygon_object->linear_mass * speed * speed + 0.5 * polygon_object->angular_mass * polygon_object->angular_velocity * polygon_object->angular_velocity;
}

double polygon_object_get_angular_velocity(PolygonObject *polygon_object)
{
    return polygon_object->angular_velocity;
}

void polygon_object_copy_world_vertex(PolygonObject *polygon_object, Vector *destination)
{
    polygon_copy_vertex(polygon_object->global, destination);
}

void polygon_object_set_position(PolygonObject *polygon_object, Vector position)
{
    polygon_object->position = position;
}

void polygon_object_translate(PolygonObject *polygon_object, Vector translation)
{
    polygon_object->position = vector_get_sum(polygon_object->position, translation);
}

void polygon_object_set_orientation(PolygonObject *polygon_object, double orientation)
{
    polygon_object->orientation = orientation;
}

void polygon_object_adjust_linear_velocity(PolygonObject *polygon_object, Vector delta)
{
    polygon_object->linear_velocity.x += delta.x;
    polygon_object->linear_velocity.y += delta.y;
}

void polygon_object_adjust_angular_velocity(PolygonObject *polygon_object, double delta)
{
    polygon_object->angular_velocity += delta;
}

void polygon_object_set_color(PolygonObject *polygon_object, Color color)
{
    polygon_object->color = color;
}

void polygon_object_update(PolygonObject *polygon_object, double delta_time)
{
    const double pi = 3.141592653589793;

    vector_add(&polygon_object->position, vector_get_scaled(polygon_object->linear_velocity, delta_time));
    polygon_object->orientation += delta_time * polygon_object->angular_velocity;

    polygon_object->orientation = fmod(polygon_object->orientation, 2.0 * pi);

    polygon_copy(polygon_object->local, polygon_object->global);
    polygon_rotate(polygon_object->global, polygon_object->orientation);
    polygon_translate(polygon_object->global, polygon_object->position);
}

void polygon_object_apply_impulse(PolygonObject *polygon_object, Vector position, Vector impulse)
{
    Vector delta_linear_velocity = vector_get_scaled(impulse, 1.0 / polygon_object->linear_mass);
    polygon_object->linear_velocity = vector_get_sum(polygon_object->linear_velocity, delta_linear_velocity);

    Vector local_position = vector_get_difference(position, polygon_object->position);
    double delta_angular_velocity = vector_get_cross_product(local_position, impulse) / polygon_object->angular_mass;
    polygon_object->angular_velocity += delta_angular_velocity;
}

void polygon_object_copy_collision_parameters(PolygonObject *polygon_object, Vector collision_position, Vector collision_direction, double *a_destination, double *b_destination)
{
    Vector local_position = vector_get_difference(collision_position, polygon_object->position);
    double base_torque = vector_get_cross_product(local_position, collision_direction);
    *a_destination = 1.0f / (2.0f * polygon_object->linear_mass) + base_torque * base_torque / (2.0f * polygon_object->angular_mass);
    *b_destination = vector_get_dot_product(collision_direction, polygon_object->linear_velocity) + base_torque * polygon_object->angular_velocity;
}

bool polygon_object_collision_handle(PolygonObject *left, PolygonObject *right, VectorObjectQueue *vector_object_queue)
{
    Vector contact_point, axis;
    if(!collision_check(left, right, &contact_point, &axis))
        return false;

    polygon_object_translate(left, axis);
    axis = vector_get_normalized(axis);

    double a_left, b_left, a_right, b_right;
    polygon_object_copy_collision_parameters(left, contact_point, axis, &a_left, &b_left);
    polygon_object_copy_collision_parameters(right, contact_point, vector_get_negative(axis), &a_right, &b_right);

    double a = a_left + a_right;
    double b = b_left + b_right;

    double impulse = -b / a;
    polygon_object_apply_impulse(left, contact_point, vector_get_scaled(axis, impulse));
    polygon_object_apply_impulse(right, contact_point, vector_get_scaled(axis, -impulse));

    VectorObject *vector_object_left = vector_object_create();
    vector_object_set_vector(vector_object_left, axis);
    vector_object_set_position(vector_object_left, contact_point);
    vector_object_set_color(vector_object_left, polygon_object_get_color(right));
    vector_object_queue_submit_vector(vector_object_queue, vector_object_left);
    vector_object_left = NULL;

    VectorObject *vector_object_right = vector_object_create();
    vector_object_set_vector(vector_object_right, vector_get_negative(axis));
    vector_object_set_position(vector_object_right, contact_point);
    vector_object_set_color(vector_object_right, polygon_object_get_color(left));
    vector_object_queue_submit_vector(vector_object_queue, vector_object_right);
    vector_object_right = NULL;

    return true;
}

void polygon_object_collision_handle_with_wall(PolygonObject *polygon, VectorObjectQueue *vector_object_queue)
{
    const double bound[] = { -6.67f, 6.67f, -5.0f, 5.0f };
    const Vector axis[] = { vector_get(1.0f, 0.0f), vector_get(-1.0f, 0.0f), vector_get(0.0f, 1.0f), vector_get(0.0f, -1.0f) };
    const Edge edge[] = {
        edge_get(vector_get(bound[LEFT], bound[BOTTOM]), vector_get(bound[LEFT], bound[TOP])),
        edge_get(vector_get(bound[RIGHT], bound[BOTTOM]), vector_get(bound[RIGHT], bound[TOP])),
        edge_get(vector_get(bound[LEFT], bound[BOTTOM]), vector_get(bound[RIGHT], bound[BOTTOM])),
        edge_get(vector_get(bound[LEFT], bound[TOP]), vector_get(bound[RIGHT], bound[TOP]))
    };

    for(int i = 0; i < 4; i++)
    {
        Vector contact_point;
        double polygon_value = collision_get_projection_min(axis[i], edge[i], polygon->global, &contact_point);
        double edge_value = -fabs(bound[i]);

        if(polygon_value < edge_value)
        {
            polygon_object_translate(polygon, vector_get_scaled(axis[i], edge_value - polygon_value));

            double a, b;
            polygon_object_copy_collision_parameters(polygon, contact_point, axis[i], &a, &b);

            double impulse = -b / a;
            polygon_object_apply_impulse(polygon, contact_point, vector_get_scaled(axis[i], impulse));

            VectorObject *vector_object = vector_object_create();
            vector_object_set_vector(vector_object, axis[i]);
            vector_object_set_position(vector_object, contact_point);
            vector_object_set_color(vector_object, color_get(1.0f, 1.0f, 1.0f));
            vector_object_queue_submit_vector(vector_object_queue, vector_object);
            vector_object = NULL;
        }
    }
}
