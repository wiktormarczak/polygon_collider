#include <polygon_collider/app.h>
#include <polygon_collider/window.h>
#include <polygon_collider/renderer.h>
#include <stdbool.h>
#include <stdlib.h>

struct App
{
    Window *window;
    Renderer *renderer;
    Polygon *polygon;
};

App *app_create()
{
    App *app = malloc(sizeof(App));

    app->window = window_create("Polygon Collider", 800, 600);
    app->renderer = renderer_create();
    app->polygon = polygon_create_triangle(color_get(1.0f, 0.0f, 0.0f));

    return app;
}

void app_destroy(App *app)
{
    window_destroy(app->window);
    renderer_destroy(app->renderer);
    polygon_destroy(app->polygon);

    free(app);
}

void app_run(App *app)
{
    bool open = true;
    while(open)
    {
        if(window_was_closed(app->window))
            open = false;

        polygon_update(app->polygon);

        renderer_clear(app->renderer);

        renderer_draw_polygon(app->renderer, app->polygon);

        window_refresh(app->window);
    }
}
