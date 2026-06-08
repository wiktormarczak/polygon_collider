#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera Camera;

Camera *camera_create(float aspect);
void camera_destroy(Camera *camera);

void camera_copy_matrix(Camera *camera, float *destination);
void camera_update(Camera *camera);

#endif
