#ifndef SHADER_H
#define SHADER_H

unsigned int shader_create_program(const char *vertex_shader_path, const char *fragment_shader_path);

unsigned int shader_create_vertex(const char *shader_path);
unsigned int shader_create_fragment(const char *shader_path);

unsigned int shader_create(const char *shader_path, unsigned int shader_type);

char *shader_get_source(const char *shader_path);

#endif
