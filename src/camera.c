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

#include <polygon_collider/camera.h>
#include <polygon_collider/vector.h>
#include <string.h>
#include <stdlib.h>

struct Camera
{
    Vector position;
    float aspect, height;
    float matrix[16];
};

Camera *camera_create(float aspect)
{
    Camera *camera = malloc(sizeof(Camera));

    camera->position.x = 0.0f;
    camera->position.y = 0.0f;

    camera->aspect = aspect;
    camera->height = 10.0f;

    int i;
    for(i = 0; i < 15; i++)
        camera->matrix[i] = 0.0f;
    camera->matrix[15] = 1.0f;

    return camera;
}

void camera_destroy(Camera *camera)
{
    free(camera);
}

void camera_copy_matrix(Camera *camera, float *destination)
{
    memcpy(destination, camera->matrix, sizeof(camera->matrix));
}

void camera_update(Camera *camera)
{
    float a = 2.0f / camera->aspect / camera->height;
    float b = 2.0f / camera->height;

    camera->matrix[0] = a;
    camera->matrix[3] = -camera->position.x * a;
    camera->matrix[5] = b;
    camera->matrix[7] = -camera->position.y * b;
}
