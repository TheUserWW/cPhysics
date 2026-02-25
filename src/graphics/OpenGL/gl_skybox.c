#include "graphics/OpenGL/gl_skybox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics/stb_image.h"

static const char* skybox_vertex_shader = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec3 TexCoords;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"void main()\n"
"{\n"
"    TexCoords = aPos;\n"
"    vec4 pos = projection * view * vec4(aPos, 1.0);\n"
"    gl_Position = pos.xyww;\n"
"}\n";

static const char* skybox_fragment_shader = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 TexCoords;\n"
"uniform samplerCube skybox;\n"
"void main()\n"
"{\n"
"    FragColor = texture(skybox, TexCoords);\n"
"}\n";

static GLuint compile_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("ERROR::SKYBOX::SHADER::COMPILATION_FAILED\n%s\n", info_log);
    }
    
    return shader;
}

static GLuint create_shader_program() {
    GLuint vertex_shader = compile_shader(skybox_vertex_shader, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(skybox_fragment_shader, GL_FRAGMENT_SHADER);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        printf("ERROR::SKYBOX::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

// 加载水平交叉布局（T形）的立方体贴图
static GLuint load_cross_cubemap(const char* texture_path, int is_hdr) {
    int width, height, channels;
    void* data;
    
    if (is_hdr) {
        data = stbi_loadf(texture_path, &width, &height, &channels, 3);
    } else {
        data = stbi_load(texture_path, &width, &height, &channels, 3);
    }
    
    if (!data) {
        printf("ERROR::SKYBOX::Failed to load cross texture: %s\n", texture_path);
        return 0;
    }
    
    printf("Using cross layout (T-shape)\n");
    
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    
    int face_width = width / 4;
    int face_height = height / 3;
    
    // 立方体贴图面在图片中的位置（水平交叉布局）
    int face_offsets[6][2] = {
        {2, 1}, // +X (右)
        {0, 1}, // -X (左)
        {1, 0}, // +Y (上)
        {1, 2}, // -Y (下)
        {1, 1}, // +Z (前)
        {3, 1}  // -Z (后)
    };
    
    float* face_data = (float*)malloc(face_width * face_height * 3 * sizeof(float));
    
    for (int i = 0; i < 6; i++) {
        int ox = face_offsets[i][0] * face_width;
        int oy = face_offsets[i][1] * face_height;
        
        for (int y = 0; y < face_height; y++) {
            for (int x = 0; x < face_width; x++) {
                int src_idx = ((oy + y) * width + (ox + x)) * 3;
                int dst_idx = (y * face_width + x) * 3;
                
                if (is_hdr) {
                    float* hdr_data = (float*)data;
                    face_data[dst_idx] = hdr_data[src_idx];
                    face_data[dst_idx + 1] = hdr_data[src_idx + 1];
                    face_data[dst_idx + 2] = hdr_data[src_idx + 2];
                } else {
                    unsigned char* ldr_data = (unsigned char*)data;
                    face_data[dst_idx] = ldr_data[src_idx] / 255.0f;
                    face_data[dst_idx + 1] = ldr_data[src_idx + 1] / 255.0f;
                    face_data[dst_idx + 2] = ldr_data[src_idx + 2] / 255.0f;
                }
            }
        }
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                     face_width, face_height, 0, GL_RGB, GL_FLOAT, face_data);
    }
    
    free(face_data);
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    return texture_id;
}

// 从方向向量采样等矩形柱状图
static void sample_equirectangular(float* data, int width, int height, int channels, 
                                    float x, float y, float z, float* result) {
    float phi = atan2f(z, x);
    float theta = acosf(y);
    
    float u = (phi + 3.1415926535f) / (2.0f * 3.1415926535f);
    float v = theta / 3.1415926535f;
    
    u = (u < 0.0f) ? 0.0f : (u > 1.0f) ? 1.0f : u;
    v = (v < 0.0f) ? 0.0f : (v > 1.0f) ? 1.0f : v;
    
    int tex_x = (int)(u * (width - 1));
    int tex_y = (int)(v * (height - 1));
    
    int idx = (tex_y * width + tex_x) * channels;
    result[0] = data[idx];
    result[1] = data[idx + 1];
    result[2] = data[idx + 2];
}

// 加载等矩形柱状图格式的立方体贴图
static GLuint load_equirectangular_cubemap(const char* texture_path, int is_hdr) {
    int width, height, channels;
    void* data;
    
    if (is_hdr) {
        data = stbi_loadf(texture_path, &width, &height, &channels, 3);
    } else {
        data = stbi_load(texture_path, &width, &height, &channels, 3);
    }
    
    if (!data) {
        printf("ERROR::SKYBOX::Failed to load equirectangular texture: %s\n", texture_path);
        return 0;
    }
    
    printf("Using equirectangular layout\n");
    
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    
    int face_size = height;
    if (face_size > 4096) face_size = 4096; // 支持4K纹理
    if (face_size < 512) face_size = 1024; // 提高最小尺寸
    
    float* face_data = (float*)malloc(face_size * face_size * 3 * sizeof(float));
    
    float face_bases[6][3][3] = {
        {{ 0, 0, -1}, { 0, -1,  0}, { 1,  0,  0}}, // +X
        {{ 0, 0,  1}, { 0, -1,  0}, {-1,  0,  0}}, // -X
        {{ 1, 0,  0}, { 0,  0,  1}, { 0,  1,  0}}, // +Y
        {{ 1, 0,  0}, { 0,  0, -1}, { 0, -1,  0}}, // -Y
        {{ 1, 0,  0}, { 0, -1,  0}, { 0,  0,  1}}, // +Z
        {{-1, 0,  0}, { 0, -1,  0}, { 0,  0, -1}}  // -Z
    };
    
    float* float_data = NULL;
    if (!is_hdr) {
        float_data = (float*)malloc(width * height * 3 * sizeof(float));
        unsigned char* ldr_data = (unsigned char*)data;
        for (int i = 0; i < width * height * 3; i++) {
            float_data[i] = ldr_data[i] / 255.0f;
        }
    }
    
    for (int face = 0; face < 6; face++) {
        for (int y = 0; y < face_size; y++) {
            for (int x = 0; x < face_size; x++) {
                float u = (2.0f * (x + 0.5f)) / face_size - 1.0f;
                float v = (2.0f * (y + 0.5f)) / face_size - 1.0f;
                
                float dir[3];
                dir[0] = face_bases[face][0][0] * u + face_bases[face][1][0] * v + face_bases[face][2][0];
                dir[1] = face_bases[face][0][1] * u + face_bases[face][1][1] * v + face_bases[face][2][1];
                dir[2] = face_bases[face][0][2] * u + face_bases[face][1][2] * v + face_bases[face][2][2];
                
                float len = sqrtf(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
                dir[0] /= len; dir[1] /= len; dir[2] /= len;
                
                float color[3];
                int dst_idx = (y * face_size + x) * 3;
                
                if (is_hdr) {
                    sample_equirectangular((float*)data, width, height, 3, dir[0], dir[1], dir[2], color);
                } else {
                    sample_equirectangular(float_data, width, height, 3, dir[0], dir[1], dir[2], color);
                }
                
                face_data[dst_idx] = color[0];
                face_data[dst_idx + 1] = color[1];
                face_data[dst_idx + 2] = color[2];
            }
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB16F, 
                     face_size, face_size, 0, GL_RGB, GL_FLOAT, face_data);
    }
    
    if (float_data) free(float_data);
    free(face_data);
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return texture_id;
}

static GLuint load_simple_cubemap(const char* texture_path) {
    int width, height, channels;
    int is_hdr = stbi_is_hdr(texture_path);
    
    // 先获取图片尺寸来判断格式
    if (!stbi_info(texture_path, &width, &height, &channels)) {
        printf("ERROR::SKYBOX::Failed to get texture info: %s\n", texture_path);
        return 0;
    }
    
    printf("Loaded texture: %dx%d, channels: %d, HDR: %s\n", 
           width, height, channels, is_hdr ? "yes" : "no");
    
    // 判断是水平交叉布局（宽高比接近 4:3）还是等矩形柱状图（宽高比接近 2:1）
    float aspect_ratio = (float)width / height;
    
    if (fabs(aspect_ratio - 4.0f/3.0f) < 0.1f && width % 4 == 0 && height % 3 == 0) {
        // 水平交叉布局
        return load_cross_cubemap(texture_path, is_hdr);
    } else {
        // 等矩形柱状图（默认用于 HDR 文件）
        return load_equirectangular_cubemap(texture_path, is_hdr);
    }
}

SkyboxRenderer* create_skybox_renderer(const char* texture_path) {
    SkyboxRenderer* renderer = (SkyboxRenderer*)malloc(sizeof(SkyboxRenderer));
    if (!renderer) {
        printf("ERROR::SKYBOX::Failed to allocate memory for renderer\n");
        return NULL;
    }
    memset(renderer, 0, sizeof(SkyboxRenderer));
    
    float skybox_vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glBindVertexArray(0);
    
    renderer->shader_program = create_shader_program();
    if (!renderer->shader_program) {
        printf("ERROR::SKYBOX::Failed to create shader program\n");
        destroy_skybox_renderer(renderer);
        return NULL;
    }
    
    printf("Loading skybox texture from: %s\n", texture_path);
    renderer->texture_id = load_simple_cubemap(texture_path);
    
    if (!renderer->texture_id) {
        printf("ERROR::SKYBOX::Failed to load skybox texture\n");
        destroy_skybox_renderer(renderer);
        return NULL;
    }
    
    printf("Skybox renderer created successfully\n");
    return renderer;
}

void destroy_skybox_renderer(SkyboxRenderer* renderer) {
    if (renderer) {
        if (renderer->vao) glDeleteVertexArrays(1, &renderer->vao);
        if (renderer->vbo) glDeleteBuffers(1, &renderer->vbo);
        if (renderer->shader_program) glDeleteProgram(renderer->shader_program);
        if (renderer->texture_id) glDeleteTextures(1, &renderer->texture_id);
        free(renderer);
    }
}

void render_skybox(SkyboxRenderer* renderer, const float* view, const float* projection) {
    if (!renderer || !renderer->texture_id) return;
    
    // 保存当前深度测试状态
    glDepthFunc(GL_LEQUAL);
    
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->vao);
    
    // 移除视图矩阵的平移部分，只保留旋转
    float view_no_translation[16];
    for (int i = 0; i < 16; i++) {
        view_no_translation[i] = view[i];
    }
    view_no_translation[12] = 0.0f;
    view_no_translation[13] = 0.0f;
    view_no_translation[14] = 0.0f;
    
    GLuint view_loc = glGetUniformLocation(renderer->shader_program, "view");
    GLuint proj_loc = glGetUniformLocation(renderer->shader_program, "projection");
    
    if (view_loc != -1) {
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view_no_translation);
    }
    if (proj_loc != -1) {
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection);
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->texture_id);
    
    GLuint skybox_loc = glGetUniformLocation(renderer->shader_program, "skybox");
    if (skybox_loc != -1) {
        glUniform1i(skybox_loc, 0);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glUseProgram(0);
    
    // 恢复深度测试状态
    glDepthFunc(GL_LESS);
}
