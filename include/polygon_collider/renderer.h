#ifndef RENDERER_H
#define RENDERER_H

#include <polygon_collider/polygon.h>
#include <polygon_collider/camera.h>

typedef struct Renderer Renderer;

Renderer *renderer_create();
void renderer_destroy(Renderer *renderer);

void renderer_clear(Renderer *renderer);
void renderer_draw_polygon(Renderer *renderer, Polygon *polygon, Camera *camera);

#endif
