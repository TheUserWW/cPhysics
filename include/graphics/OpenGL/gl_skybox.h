#ifndef CPHYSICS_GL_SKYBOX_H
#define CPHYSICS_GL_SKYBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct SkyboxRenderer {
    GLuint vao;
    GLuint vbo;
    GLuint shader_program;
    GLuint texture_id;
} SkyboxRenderer;

SkyboxRenderer* create_skybox_renderer(const char* texture_path);
void destroy_skybox_renderer(SkyboxRenderer* renderer);
void render_skybox(SkyboxRenderer* renderer, const float* view, const float* projection);

#ifdef __cplusplus
}
#endif

#endif
