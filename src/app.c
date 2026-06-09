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

#define TITLE "Polygon Collider"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <polygon_collider/app.h>
#include <polygon_collider/window.h>
#include <polygon_collider/stopwatch.h>
#include <polygon_collider/renderer.h>
#include <polygon_collider/camera.h>
#include <polygon_collider/polygon.h>
#include <polygon_collider/collision.h>

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

struct App
{
    Window *window;
    Stopwatch *stopwatch;

    Renderer *renderer;
    Camera *camera;

    unsigned int polygon_count;
    Polygon *triangle, *square;

    bool open;
};

static void app_input(App *app);
static void app_update(App *app, float delta_time);
static void app_draw(App *app);

App *app_create()
{
    srand(time(NULL));

    App *app = malloc(sizeof(App));

    app->window = window_create(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    app->stopwatch = stopwatch_create();

    app->renderer = renderer_create();
    app->camera = camera_create((float)800 / (float)600);

    app->triangle = polygon_create_regular(3, 1.0f, color_get(1.0f, 0.0f, 0.0f));
    polygon_set_position(app->triangle, vector_get(-3.0f, 0.5f));
    polygon_adjust_linear_velocity(app->triangle, vector_get(1.0f, 0.0f));

    app->square = polygon_create_regular(4, 1.0f, color_get(0.0f, 0.0f, 1.0f));
    polygon_set_position(app->square, vector_get(3.0f, -0.5f));
    polygon_adjust_linear_velocity(app->square, vector_get(-1.0f, 0.0f));

    return app;
}

void app_destroy(App *app)
{
    polygon_destroy(app->triangle);
    app->triangle = NULL;

    polygon_destroy(app->square);
    app->square = NULL;

    camera_destroy(app->camera);
    app->camera = NULL;

    renderer_destroy(app->renderer);
    app->renderer = NULL;

    stopwatch_destroy(app->stopwatch);
    app->stopwatch = NULL;

    window_destroy(app->window);
    app->window = NULL;

    free(app);
}

void app_run(App *app)
{
    app->open = true;
    while(app->open)
    {
        float delta_time = stopwatch_get_delta(app->stopwatch);

        app_input(app);
        app_update(app, delta_time);
        app_draw(app);

        if(delta_time < 1 / 60.0f)
            stopwatch_delay(app->stopwatch, 1 / 60.0f - delta_time);
    }
}

static void app_input(App *app)
{
    if(window_was_closed(app->window))
        app->open = false;
}

static void app_update(App *app, float delta_time)
{
    polygon_update(app->triangle, delta_time);
    polygon_update(app->square, delta_time);

    Vector contact_point, axis;
    if(collision_check(app->triangle, app->square, &contact_point, &axis))
        polygon_set_color(app->triangle, color_get(1.0f, 1.0f, 1.0f));
    else
        polygon_set_color(app->triangle, color_get(1.0f, 0.0f, 0.0f));

    camera_update(app->camera);
}

static void app_draw(App *app)
{
    renderer_submit(app->renderer, app->triangle);
    renderer_submit(app->renderer, app->square);

    renderer_flush(app->renderer, app->camera);
    window_refresh(app->window);
}
