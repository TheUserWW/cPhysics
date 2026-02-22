//
// Created by wcx16 on 2026/2/16.
//

#include "graphics/OpenGL/gl_sphere.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

// 顶点着色器源码
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

// 片段着色器源码
static const char* fragment_shader_source = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 objectColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(objectColor, 1.0);\n"
"}\0";

// 编译着色器
GLuint compile_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // 检查编译错误
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
    }
    
    return shader;
}

// 创建着色器程序
GLuint create_shader_program() {
    GLuint vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    // 检查链接错误
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

// 生成球体顶点数据
static void generate_sphere_vertices(float* vertices, unsigned int* indices, 
                                    float radius, int stacks, int slices, 
                                    int* vertex_count, int* index_count) {
    *vertex_count = (stacks + 1) * (slices + 1);
    *index_count = stacks * slices * 6;
    
    // 生成顶点
    int vertex_index = 0;
    for (int i = 0; i <= stacks; ++i) {
        float phi = (float)i / stacks * M_PI;
        for (int j = 0; j <= slices; ++j) {
            float theta = (float)j / slices * 2 * M_PI;
            
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);
            
            vertices[vertex_index * 3] = x;
            vertices[vertex_index * 3 + 1] = y;
            vertices[vertex_index * 3 + 2] = z;
            vertex_index++;
        }
    }
    
    // 生成索引
    int index_index = 0;
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;
            
            indices[index_index++] = first;
            indices[index_index++] = second;
            indices[index_index++] = first + 1;
            
            indices[index_index++] = first + 1;
            indices[index_index++] = second;
            indices[index_index++] = second + 1;
        }
    }
}

// 创建球体渲染器
SphereRenderer* create_sphere_renderer(float radius, int stacks, int slices) {
    if (stacks < 3) stacks = 3;
    if (slices < 3) slices = 3;
    
    SphereRenderer* renderer = (SphereRenderer*)malloc(sizeof(SphereRenderer));
    if (!renderer) {
        return NULL;
    }
    
    memset(renderer, 0, sizeof(SphereRenderer));
    renderer->radius = radius;
    renderer->stacks = stacks;
    renderer->slices = slices;
    
    // 生成顶点数据
    int vertex_count, index_count;
    float* vertices = (float*)malloc(sizeof(float) * (stacks + 1) * (slices + 1) * 3);
    unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * stacks * slices * 6);
    
    generate_sphere_vertices(vertices, indices, radius, stacks, slices, 
                            &vertex_count, &index_count);
    
    renderer->vertex_count = vertex_count;
    renderer->index_count = index_count;
    
    // 创建着色器程序
    renderer->shader_program = create_shader_program();
    
    // 创建VAO, VBO, EBO
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glGenBuffers(1, &renderer->ebo);
    
    glBindVertexArray(renderer->vao);
    
    // 绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count * 3, vertices, GL_STATIC_DRAW);
    
    // 绑定索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, indices, GL_STATIC_DRAW);
    
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    free(vertices);
    free(indices);
    
    return renderer;
}

// 销毁球体渲染器
void destroy_sphere_renderer(SphereRenderer* renderer) {
    if (renderer) {
        if (renderer->vao) glDeleteVertexArrays(1, &renderer->vao);
        if (renderer->vbo) glDeleteBuffers(1, &renderer->vbo);
        if (renderer->ebo) glDeleteBuffers(1, &renderer->ebo);
        if (renderer->shader_program) glDeleteProgram(renderer->shader_program);
        free(renderer);
    }
}

// 渲染球体
void render_sphere(const SphereRenderer* renderer, const Entity* entity) {
    if (!renderer || !entity) return;
    
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->vao);
    
    // 设置模型矩阵（基于实体位置）- OpenGL使用列主序
    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        (float)entity->position.x, (float)entity->position.y, (float)entity->position.z, 1.0f
    };
    
    GLuint model_loc = glGetUniformLocation(renderer->shader_program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
    
    // 设置视图和投影矩阵（需要外部设置）
    // 这些矩阵应该由相机系统提供
    
    // 设置物体颜色（基于实体属性）
    float color[3];
    if (entity->is_static) {
        color[0] = 0.5f; color[1] = 0.5f; color[2] = 0.5f; // 灰色表示静态物体
    } else if (entity->charge > 0) {
        color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; // 红色表示正电荷
    } else if (entity->charge < 0) {
        color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f; // 蓝色表示负电荷
    } else {
        color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f; // 绿色表示中性物体
    }
    
    GLuint color_loc = glGetUniformLocation(renderer->shader_program, "objectColor");
    glUniform3f(color_loc, color[0], color[1], color[2]);
    
    // 绘制球体
    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

// 设置球体半径
void set_sphere_radius(SphereRenderer* renderer, float radius) {
    if (renderer && radius > 0 && radius != renderer->radius) {
        int stacks = renderer->stacks;
        int slices = renderer->slices;
        
        glDeleteVertexArrays(1, &renderer->vao);
        glDeleteBuffers(1, &renderer->vbo);
        glDeleteBuffers(1, &renderer->ebo);
        
        renderer->radius = radius;
        
        int vertex_count, index_count;
        float* vertices = (float*)malloc(sizeof(float) * (stacks + 1) * (slices + 1) * 3);
        unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * stacks * slices * 6);
        
        generate_sphere_vertices(vertices, indices, radius, stacks, slices, 
                                &vertex_count, &index_count);
        
        renderer->vertex_count = vertex_count;
        renderer->index_count = index_count;
        
        glGenVertexArrays(1, &renderer->vao);
        glGenBuffers(1, &renderer->vbo);
        glGenBuffers(1, &renderer->ebo);
        
        glBindVertexArray(renderer->vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count * 3, vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
        
        free(vertices);
        free(indices);
    }
}
