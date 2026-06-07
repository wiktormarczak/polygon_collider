#include <polygon_collider/app.h>
#include <polygon_collider/window.h>
#include <polygon_collider/renderer.h>
#include <stdbool.h>
#include <stdlib.h>

struct App
{
    Window *window;
};

App *app_create()
{
    App *app = malloc(sizeof(App));

    app->window = window_create("Polygon Collider", 800, 600);

    return app;
}

void app_destroy(App *app)
{
    window_destroy(app->window);
}

void app_run(App *app)
{
    bool open = true;
    while(open)
    {
        if(window_was_closed(app->window))
            open = false;

        renderer_clear();

        window_refresh(app->window);
    }
}
