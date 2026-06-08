#include <polygon_collider/app.h>
#include <polygon_collider/window.h>
#include <polygon_collider/renderer.h>
#include <polygon_collider/camera.h>
#include <polygon_collider/timer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct App
{
    Window *window;
    Renderer *renderer;
    Camera *camera;
    Timer *timer;

    unsigned int polygon_count;
    Polygon **polygon;
};

App *app_create()
{
    App *app = malloc(sizeof(App));

    app->window = window_create("Polygon Collider", 800, 600);
    app->renderer = renderer_create();
    app->camera = camera_create((float)800 / (float)600);
    app->timer = timer_create();

    srand(time(NULL));
    app->polygon_count = 8192;
    app->polygon = malloc(app->polygon_count * sizeof(Polygon *));
    int i;
    for(i = 0; i < app->polygon_count; i++)
    {
        app->polygon[i] = polygon_create_regular(3 + rand() % 6, 1.0f, color_get((rand() % 11) / 10.0f, (rand() % 11) / 10.0f, (rand() %11) / 10.0f));
        polygon_adjust_linear_velocity(app->polygon[i], vector_get((rand() % 11 - 5) / 10.0f, (rand() % 11 - 5) / 10.0f));
        polygon_adjust_angular_velocity(app->polygon[i], (rand() % 11 - 5) / 10.0f);
    }

    return app;
}

void app_destroy(App *app)
{
    renderer_destroy(app->renderer);
    camera_destroy(app->camera);
    timer_destroy(app->timer);
    window_destroy(app->window);

    int i;
    for(i = 0; i < app->polygon_count; i++)
        polygon_destroy(app->polygon[i]);
    free(app->polygon);

    free(app);
}

void app_run(App *app)
{
    bool open = true;
    while(open)
    {
        float delta_time = timer_get_delta(app->timer);
        printf("FPS: %.2f\n", 1.0f / delta_time);

        if(window_was_closed(app->window))
            open = false;

        int i;
        for(i = 0; i < app->polygon_count; i++)
            polygon_update(app->polygon[i], delta_time);

        camera_update(app->camera);

        renderer_clear(app->renderer);

        for(i = 0; i < app->polygon_count; i++)
            renderer_submit_polygon(app->renderer, app->polygon[i]);
        renderer_flush(app->renderer, app->camera);

        window_refresh(app->window);
    }
}
