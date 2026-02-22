//
// Created by wcx16 on 2026/2/21.
//

#ifndef CPHYSICS_GL_CUBE_H
#define CPHYSICS_GL_CUBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/entity.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct CubeRenderer {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint shader_program;
    GLuint textured_shader_program;
    int index_count;
    float size;
} CubeRenderer;

CubeRenderer* create_cube_renderer(float size);
void destroy_cube_renderer(CubeRenderer* renderer);
void render_cube(const CubeRenderer* renderer, const Entity* entity);
void render_cube_textured(const CubeRenderer* renderer, const Entity* entity, GLuint texture_id);
void set_cube_size(CubeRenderer* renderer, float size);

GLuint compile_cube_shader(const char* source, GLenum type);
GLuint create_cube_shader_program();
GLuint create_textured_cube_shader_program();

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_GL_CUBE_H
