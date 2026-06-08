#include <polygon_collider/app.h>
#include <polygon_collider/window.h>
#include <polygon_collider/renderer.h>
#include <polygon_collider/camera.h>
#include <stdbool.h>
#include <stdlib.h>

struct App
{
    Window *window;
    Renderer *renderer;
    Polygon *polygon;
    Camera *camera;
};

App *app_create()
{
    App *app = malloc(sizeof(App));

    app->window = window_create("Polygon Collider", 800, 600);
    app->renderer = renderer_create();
    app->polygon = polygon_create_regular(8, 0.5f, color_get(1.0f, 0.0f, 0.0f));
    app->camera = camera_create((float)800 / (float)600);

    return app;
}

void app_destroy(App *app)
{
    window_destroy(app->window);
    renderer_destroy(app->renderer);
    polygon_destroy(app->polygon);
    camera_destroy(app->camera);

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
        camera_update(app->camera);

        renderer_clear(app->renderer);

        renderer_draw_polygon(app->renderer, app->polygon, app->camera);

        window_refresh(app->window);
    }
}
