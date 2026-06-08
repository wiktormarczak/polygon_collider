#include <polygon_collider/renderer.h>
#include <polygon_collider/shader.h>
#include <polygon_collider/polygon.h>
#include <glad/gl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct Renderer
{
    unsigned int shader_program;
    unsigned int color_location, matrix_location;

    unsigned int vertex_array;

    unsigned int vertex_buffer, element_buffer;
    unsigned int submitted_to_vertex_buffer, submitted_to_element_buffer;
};

Renderer *renderer_create()
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->shader_program = shader_create_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    glUseProgram(renderer->shader_program);

    renderer->color_location = glGetUniformLocation(renderer->shader_program, "color");
    renderer->matrix_location = glGetUniformLocation(renderer->shader_program, "matrix");

    glGenVertexArrays(1, &renderer->vertex_array);
    glBindVertexArray(renderer->vertex_array);

    glGenBuffers(1, &renderer->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 100000 * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer->element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 300000 * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return renderer;
}

void renderer_destroy(Renderer *renderer)
{
    glDeleteBuffers(1, &renderer->vertex_buffer);
    glDeleteBuffers(1, &renderer->element_buffer);
    free(renderer);
}

void renderer_clear(Renderer *renderer)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_submit_polygon(Renderer* renderer, Polygon *polygon)
{
    int i;

    unsigned int vertex_count = polygon_get_vertex_count(polygon);
    Vector *vertex = malloc(vertex_count * sizeof(Vector));
    polygon_copy_world_vertex(polygon, vertex);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, renderer->submitted_to_vertex_buffer * sizeof(Vector), vertex_count * sizeof(Vector), vertex);
    renderer->submitted_to_vertex_buffer += vertex_count;

    free(vertex);

    unsigned int index_count = 3 * (vertex_count - 2);
    unsigned int *index = malloc(index_count * sizeof(unsigned int));
    for(i = 0; i < vertex_count - 2; i++)
    {
        index[3 * i] = renderer->submitted_to_element_buffer;
        index[3 * i + 1] = renderer->submitted_to_element_buffer + i + 1;
        index[3 * i + 2] = renderer->submitted_to_element_buffer + i + 2;
    }

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, renderer->submitted_to_element_buffer * sizeof(unsigned int), index_count * sizeof(unsigned int), index);
    renderer->submitted_to_element_buffer += index_count;

    free(index);
}

void renderer_flush(Renderer *renderer, Camera *camera)
{
    float matrix[16];
    camera_copy_matrix(camera, matrix);
    glUniformMatrix4fv(renderer->matrix_location, 1, true, matrix);

    glUniform3f(renderer->color_location, 1.0f, 1.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, renderer->submitted_to_element_buffer, GL_UNSIGNED_INT, 0);

    renderer->submitted_to_vertex_buffer = 0;
    renderer->submitted_to_element_buffer = 0;

    glBufferData(GL_ARRAY_BUFFER, 100000 * sizeof(Vector), NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 300000 * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
}
