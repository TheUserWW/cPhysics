//
// Created by wcx16 on 2026/2/16.
//

#ifndef CPHYSICS_GL_SPHERE_H
#define CPHYSICS_GL_SPHERE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/entity.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 球体渲染器结构体
typedef struct SphereRenderer {
    GLuint vao;           // 顶点数组对象
    GLuint vbo;           // 顶点缓冲区对象
    GLuint ebo;           // 元素缓冲区对象
    GLuint shader_program; // 着色器程序
    int vertex_count;     // 顶点数量
    int index_count;      // 索引数量
    float radius;         // 球体半径
    int stacks;           // 球体堆叠数
    int slices;           // 球体切片数
} SphereRenderer;

// 函数声明
SphereRenderer* create_sphere_renderer(float radius, int stacks, int slices);
void destroy_sphere_renderer(SphereRenderer* renderer);
void render_sphere(const SphereRenderer* renderer, const Entity* entity);
void set_sphere_radius(SphereRenderer* renderer, float radius);

// 着色器相关函数
GLuint compile_shader(const char* source, GLenum type);
GLuint create_shader_program();

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_GL_SPHERE_H