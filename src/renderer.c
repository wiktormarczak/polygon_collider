#include <polygon_collider/renderer.h>
#include <polygon_collider/shader.h>
#include <glad/gl.h>
#include <stdlib.h>

struct Renderer
{
    unsigned int shader_program;
};

Renderer *renderer_create()
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->shader_program = shader_create_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    glUseProgram(renderer->shader_program);

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
