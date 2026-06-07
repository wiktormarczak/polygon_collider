#include <polygon_collider/app.h>
#include <polygon_collider/renderer.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

struct App
{
    Renderer *renderer;
};

App *app_create()
{
    App *app = malloc(sizeof(App));

    app->renderer = renderer_create("Polygon Collider", 800, 600);

    return app;
}

void app_destroy(App *app)
{
    renderer_destroy(app->renderer);
}

void app_run(App *app)
{
    bool open = true;
    while(open)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
            if(event.type == SDL_EVENT_QUIT)
                open = false;

        renderer_clear(app->renderer);

        renderer_refresh(app->renderer);
    }
}
