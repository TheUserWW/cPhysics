//
// Created by wcx16 on 2026/2/21.
//

#ifndef CPHYSICS_GL_TEXTURE_H
#define CPHYSICS_GL_TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct Texture {
    GLuint id;
    int width;
    int height;
    int channels;
} Texture;

Texture* load_texture(const char* filepath);
void destroy_texture(Texture* texture);
void bind_texture(const Texture* texture, GLuint unit);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_GL_TEXTURE_H
