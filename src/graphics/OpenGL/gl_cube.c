//
// Created by wcx16 on 2026/2/21.
//

#include "graphics/OpenGL/gl_cube.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* vertex_shader_source = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

static const char* fragment_shader_source = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 objectColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(objectColor, 1.0);\n"
"}\0";

static const char* textured_vertex_shader_source = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"    TexCoord = aTexCoord;\n"
"}\0";

static const char* textured_fragment_shader_source = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"uniform vec3 objectColor;\n"
"void main()\n"
"{\n"
"    FragColor = texture(texture1, TexCoord) * vec4(objectColor, 1.0);\n"
"}\0";

GLuint compile_cube_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
    }
    
    return shader;
}

GLuint create_cube_shader_program() {
    GLuint vertex_shader = compile_cube_shader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_cube_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

GLuint create_textured_cube_shader_program() {
    GLuint vertex_shader = compile_cube_shader(textured_vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_cube_shader(textured_fragment_shader_source, GL_FRAGMENT_SHADER);
    
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

static void generate_cube_vertices(float* vertices, unsigned int* indices, 
                                   float size, int* index_count) {
    float half = size / 2.0f;
    
    vertices[0] = -half; vertices[1] = -half; vertices[2] =  half;
    vertices[3] =  half; vertices[4] = -half; vertices[5] =  half;
    vertices[6] =  half; vertices[7] =  half; vertices[8] =  half;
    vertices[9] = -half; vertices[10] =  half; vertices[11] =  half;
    
    vertices[12] = -half; vertices[13] = -half; vertices[14] = -half;
    vertices[15] =  half; vertices[16] = -half; vertices[17] = -half;
    vertices[18] =  half; vertices[19] =  half; vertices[20] = -half;
    vertices[21] = -half; vertices[22] =  half; vertices[23] = -half;
    
    unsigned int cube_indices[] = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        5, 4, 7, 7, 6, 5,
        4, 0, 3, 3, 7, 4,
        3, 2, 6, 6, 7, 3,
        4, 5, 1, 1, 0, 4
    };
    
    memcpy(indices, cube_indices, sizeof(cube_indices));
    *index_count = 36;
}

static void generate_textured_cube_vertices(float* vertices, unsigned int* indices, 
                                            float size, int* index_count) {
    float half = size / 2.0f;
    
    float textured_vertices[] = {
        -half, -half,  half,  0.0f, 0.0f,
         half, -half,  half,  size, 0.0f,
         half,  half,  half,  size, size,
        -half,  half,  half,  0.0f, size,
        
        -half, -half, -half,  size, 0.0f,
         half, -half, -half,  0.0f, 0.0f,
         half,  half, -half,  0.0f, size,
        -half,  half, -half,  size, size,
        
        -half,  half, -half,  0.0f, size,
         half,  half, -half,  size, size,
         half,  half,  half,  size, 0.0f,
        -half,  half,  half,  0.0f, 0.0f,
        
        -half, -half, -half,  0.0f, 0.0f,
         half, -half, -half,  size, 0.0f,
         half, -half,  half,  size, size,
        -half, -half,  half,  0.0f, size,
        
         half, -half, -half,  0.0f, 0.0f,
         half,  half, -half,  0.0f, size,
         half,  half,  half,  size, size,
         half, -half,  half,  size, 0.0f,
        
        -half, -half, -half,  size, 0.0f,
        -half,  half, -half,  size, size,
        -half,  half,  half,  0.0f, size,
        -half, -half,  half,  0.0f, 0.0f
    };
    
    memcpy(vertices, textured_vertices, sizeof(textured_vertices));
    
    unsigned int cube_indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    
    memcpy(indices, cube_indices, sizeof(cube_indices));
    *index_count = 36;
}

CubeRenderer* create_cube_renderer(float size) {
    if (size <= 0) size = 1.0f;
    
    CubeRenderer* renderer = (CubeRenderer*)malloc(sizeof(CubeRenderer));
    if (!renderer) {
        return NULL;
    }
    
    memset(renderer, 0, sizeof(CubeRenderer));
    renderer->size = size;
    
    float vertices[120];
    unsigned int indices[36];
    int index_count;
    
    generate_textured_cube_vertices(vertices, indices, size, &index_count);
    renderer->index_count = index_count;
    
    renderer->shader_program = create_cube_shader_program();
    renderer->textured_shader_program = create_textured_cube_shader_program();
    
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glGenBuffers(1, &renderer->ebo);
    
    glBindVertexArray(renderer->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    return renderer;
}

void destroy_cube_renderer(CubeRenderer* renderer) {
    if (renderer) {
        if (renderer->vao) glDeleteVertexArrays(1, &renderer->vao);
        if (renderer->vbo) glDeleteBuffers(1, &renderer->vbo);
        if (renderer->ebo) glDeleteBuffers(1, &renderer->ebo);
        if (renderer->shader_program) glDeleteProgram(renderer->shader_program);
        if (renderer->textured_shader_program) glDeleteProgram(renderer->textured_shader_program);
        free(renderer);
    }
}

void render_cube(const CubeRenderer* renderer, const Entity* entity) {
    if (!renderer || !entity) return;
    
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->vao);
    
    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        (float)entity->position.x, (float)entity->position.y, (float)entity->position.z, 1.0f
    };
    
    GLuint model_loc = glGetUniformLocation(renderer->shader_program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
    
    float color[3];
    if (entity->is_static) {
        color[0] = 0.5f; color[1] = 0.5f; color[2] = 0.5f;
    } else if (entity->charge > 0) {
        color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
    } else if (entity->charge < 0) {
        color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
    } else {
        color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
    }
    
    GLuint color_loc = glGetUniformLocation(renderer->shader_program, "objectColor");
    glUniform3f(color_loc, color[0], color[1], color[2]);
    
    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void render_cube_textured(const CubeRenderer* renderer, const Entity* entity, GLuint texture_id) {
    if (!renderer || !entity) return;
    
    glUseProgram(renderer->textured_shader_program);
    glBindVertexArray(renderer->vao);
    
    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        (float)entity->position.x, (float)entity->position.y, (float)entity->position.z, 1.0f
    };
    
    GLuint model_loc = glGetUniformLocation(renderer->textured_shader_program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
    
    GLuint color_loc = glGetUniformLocation(renderer->textured_shader_program, "objectColor");
    glUniform3f(color_loc, 1.0f, 1.0f, 1.0f);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    GLuint tex_loc = glGetUniformLocation(renderer->textured_shader_program, "texture1");
    glUniform1i(tex_loc, 0);
    
    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void set_cube_size(CubeRenderer* renderer, float size) {
    if (renderer && size > 0 && size != renderer->size) {
        glDeleteVertexArrays(1, &renderer->vao);
        glDeleteBuffers(1, &renderer->vbo);
        glDeleteBuffers(1, &renderer->ebo);
        
        renderer->size = size;
        
        float vertices[120];
        unsigned int indices[36];
        int index_count;
        
        generate_textured_cube_vertices(vertices, indices, size, &index_count);
        renderer->index_count = index_count;
        
        glGenVertexArrays(1, &renderer->vao);
        glGenBuffers(1, &renderer->vbo);
        glGenBuffers(1, &renderer->ebo);
        
        glBindVertexArray(renderer->vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }
}
