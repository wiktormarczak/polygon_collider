#include <polygon_collider/renderer.h>
#include <polygon_collider/shader.h>
#include <polygon_collider/polygon.h>
#include <glad/gl.h>
#include <stdlib.h>
#include <stdbool.h>

struct Renderer
{
    unsigned int shader_program;
    unsigned int color_location;

    unsigned int vertex_array;
};

Renderer *renderer_create()
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->shader_program = shader_create_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    glUseProgram(renderer->shader_program);

    renderer->color_location = glGetUniformLocation(renderer->shader_program, "color");

    glGenVertexArrays(1, &renderer->vertex_array);
    glBindVertexArray(renderer->vertex_array);
    glEnableVertexAttribArray(0);

    return renderer;
}

void renderer_destroy(Renderer *renderer)
{
    free(renderer);
}

void renderer_clear(Renderer *renderer)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_draw_polygon(Renderer *renderer, Polygon *polygon)
{
    Color color = polygon_get_color(polygon);
    glBindBuffer(GL_ARRAY_BUFFER, polygon_get_vertex_buffer(polygon));
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)0);
    glUniform3f(renderer->color_location, color.r, color.g, color.b);
    glDrawArrays(GL_TRIANGLE_FAN, 0, polygon_get_vertex_count(polygon));
}
