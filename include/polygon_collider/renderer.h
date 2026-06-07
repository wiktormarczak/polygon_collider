#ifndef RENDERER_H
#define RENDERER_H

typedef struct Renderer Renderer;

Renderer *renderer_create();
void renderer_destroy(Renderer *renderer);

void renderer_refresh(Renderer *renderer);

#endif
