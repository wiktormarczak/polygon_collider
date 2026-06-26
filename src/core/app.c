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

#include <polygon_collider/core/app.h>
#include <polygon_collider/system/window.h>
#include <polygon_collider/system/stopwatch.h>
#include <polygon_collider/graphics/renderer.h>
#include <polygon_collider/graphics/camera.h>
#include <polygon_collider/core/polygon_object.h>
#include <polygon_collider/core/vector_object_queue.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct App
{
    Window *window;
    Stopwatch *stopwatch;

    Renderer *renderer;
    Camera *camera;

    PolygonObject *polygon_left, *polygon_right;

    VectorObjectQueue *vector_object_queue;

    bool open, freeze;
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

    app->polygon_left = polygon_object_create_regular(3, 2.0f, color_get(0.0f, 0.0f, 1.0f));
    polygon_object_set_orientation(app->polygon_left, 3.14f / 4.0f);
    polygon_object_set_position(app->polygon_left, vector_get(-2.0f, 0.0f));
    polygon_object_adjust_linear_velocity(app->polygon_left, vector_get(1.0f, 0.0f));

    app->polygon_right = polygon_object_create_regular(4, 1.5f, color_get(0.0f, 1.0f, 0.0f));
    polygon_object_set_orientation(app->polygon_right, 3.14f / 2.0f);
    polygon_object_set_position(app->polygon_right, vector_get(2.0f, 1.0f));
    polygon_object_adjust_linear_velocity(app->polygon_right, vector_get(-1.0f, 0.0f));

    app->vector_object_queue = vector_object_queue_create();

    return app;
}

void app_destroy(App *app)
{
    vector_object_queue_destroy(app->vector_object_queue);
    app->vector_object_queue = NULL;

    polygon_object_destroy(app->polygon_left);
    app->polygon_left = NULL;

    polygon_object_destroy(app->polygon_right);
    app->polygon_right = NULL;

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
    app->freeze = false;
    while(app->open)
    {
        float delta_time = stopwatch_measure(app->stopwatch);
        stopwatch_reset(app->stopwatch);

        app_input(app);
        if(!app->freeze) app_update(app, delta_time);
        app_draw(app);

        float curr_time = stopwatch_measure(app->stopwatch);
        if(curr_time < 1 / 60.0f)
            stopwatch_wait(app->stopwatch, 1 / 60.0f - curr_time);
    }
}

static void app_input(App *app)
{
    if(window_was_closed(app->window))
        app->open = false;
}

static void app_update(App *app, float delta_time)
{
    polygon_object_update(app->polygon_left, delta_time);
    polygon_object_update(app->polygon_right, delta_time);

    polygon_object_collision_handle_with_wall(app->polygon_left, app->vector_object_queue);
    polygon_object_collision_handle_with_wall(app->polygon_right, app->vector_object_queue);
    polygon_object_collision_handle(app->polygon_left, app->polygon_right, app->vector_object_queue);
    vector_object_queue_update(app->vector_object_queue, delta_time);

    camera_update(app->camera);
}

static void app_draw(App *app)
{
    renderer_submit_polygon(app->renderer, app->polygon_left);
    renderer_submit_polygon(app->renderer, app->polygon_right);
    renderer_submit_vector_queue(app->renderer, app->vector_object_queue);

    renderer_flush(app->renderer, app->camera);
    window_refresh(app->window);
}
