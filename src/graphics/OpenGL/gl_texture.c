//
// Created by wcx16 on 2026/2/21.
//

#include "graphics/OpenGL/gl_texture.h"
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "graphics/stb_image.h"

Texture* load_texture(const char* filepath) {
    Texture* texture = (Texture*)malloc(sizeof(Texture));
    if (!texture) {
        printf("Failed to allocate memory for texture\n");
        return NULL;
    }
    
    stbi_set_flip_vertically_on_load(1);
    
    unsigned char* data = stbi_load(filepath, &texture->width, &texture->height, &texture->channels, 0);
    if (!data) {
        printf("Failed to load texture: %s\n", filepath);
        free(texture);
        return NULL;
    }
    
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format;
    if (texture->channels == 1)
        format = GL_RED;
    else if (texture->channels == 3)
        format = GL_RGB;
    else if (texture->channels == 4)
        format = GL_RGBA;
    else
        format = GL_RGB;
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    printf("Texture loaded: %s (%dx%d, %d channels)\n", filepath, texture->width, texture->height, texture->channels);
    
    return texture;
}

void destroy_texture(Texture* texture) {
    if (texture) {
        if (texture->id) {
            glDeleteTextures(1, &texture->id);
        }
        free(texture);
    }
}

void bind_texture(const Texture* texture, GLuint unit) {
    if (texture) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }
}
