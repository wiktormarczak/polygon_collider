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
