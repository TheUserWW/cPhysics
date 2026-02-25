#define _USE_MATH_DEFINES
#include "graphics/OpenGL/gl_sphere.hpp"
#include <cmath>
#include <cstring>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform float charge;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 specular = 0.5 * spec * vec3(1.0, 1.0, 1.0);
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation error: %s\n", infoLog);
        return 0;
    }
    return shader;
}

static GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    if (!vertexShader || !fragmentShader) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Shader program linking error: %s\n", infoLog);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

Sphere::Sphere() : radius(1.0) {
    std::memset(name, 0, sizeof(name));
    mass = 0.0;
    charge = 0.0;
    position = {0, 0, 0};
    velocity = {0, 0, 0};
    acceleration = {0, 0, 0};
    quaternion = quat_identity();
    angular_velocity = {0, 0, 0};
    angular_acceleration = {0, 0, 0};
    moment_of_inertia = 1.0;
    coefficient_of_restitution = 0.8;
    rigid_body = true;
    is_static = false;
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
}

Sphere::Sphere(const char* n, double m, double c, 
               const Vector& pos, const Vector& vel,
               double r, const float col[3]) : radius(r) {
    std::strncpy(name, n, 255);
    name[255] = '\0';
    mass = m;
    charge = c;
    position = pos;
    velocity = vel;
    acceleration = {0, 0, 0};
    quaternion = quat_identity();
    angular_velocity = {0, 0, 0};
    angular_acceleration = {0, 0, 0};
    moment_of_inertia = 1.0;
    coefficient_of_restitution = 0.8;
    rigid_body = true;
    is_static = false;
    
    if (col) {
        color[0] = col[0];
        color[1] = col[1];
        color[2] = col[2];
    } else {
        if (charge > 0) {
            color[0] = 1.0f; color[1] = 0.3f; color[2] = 0.3f;
        } else if (charge < 0) {
            color[0] = 0.3f; color[1] = 0.3f; color[2] = 1.0f;
        } else {
            color[0] = 0.3f; color[1] = 1.0f; color[2] = 0.3f;
        }
    }
}

void Sphere::setColor(float r, float g, float b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Sphere::getColor(float* outColor) const {
    outColor[0] = color[0];
    outColor[1] = color[1];
    outColor[2] = color[2];
}

double Sphere::getVolume() const {
    return (4.0 / 3.0) * M_PI * radius * radius * radius;
}

double Sphere::getDensity() const {
    double vol = getVolume();
    return vol > 0 ? mass / vol : 0.0;
}

double Sphere::getSurfaceArea() const {
    return 4.0 * M_PI * radius * radius;
}

SphereRenderer::SphereRenderer() 
    : shader_program(0), vao(0), vbo(0), ebo(0), indexCount(0), baseRadius(1.0f) {}

SphereRenderer::~SphereRenderer() {
    cleanup();
}

bool SphereRenderer::init(float radius, unsigned int sectors, unsigned int stacks) {
    baseRadius = radius;
    
    shader_program = createShaderProgram();
    if (!shader_program) {
        return false;
    }
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float sectorStep = 2 * M_PI / sectors;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;
    
    for (unsigned int i = 0; i <= stacks; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);
        
        for (unsigned int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;
            
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }
    
    unsigned int k1, k2;
    for (unsigned int i = 0; i < stacks; ++i) {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;
        
        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    
    indexCount = static_cast<unsigned int>(indices.size());
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    return true;
}

void SphereRenderer::cleanup() {
    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (ebo) {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
    if (shader_program) {
        glDeleteProgram(shader_program);
        shader_program = 0;
    }
}

void SphereRenderer::render(const Sphere* sphere, const float* view, const float* projection) {
    if (!sphere || !shader_program) return;
    
    glUseProgram(shader_program);
    
    float model[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        static_cast<float>(sphere->position.x),
        static_cast<float>(sphere->position.y),
        static_cast<float>(sphere->position.z),
        1
    };
    
    GLuint modelLoc = glGetUniformLocation(shader_program, "model");
    GLuint viewLoc = glGetUniformLocation(shader_program, "view");
    GLuint projLoc = glGetUniformLocation(shader_program, "projection");
    GLuint colorLoc = glGetUniformLocation(shader_program, "objectColor");
    GLuint lightPosLoc = glGetUniformLocation(shader_program, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shader_program, "viewPos");
    GLuint chargeLoc = glGetUniformLocation(shader_program, "charge");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
    glUniform3fv(colorLoc, 1, sphere->color);
    glUniform3f(lightPosLoc, 50.0f, 50.0f, 50.0f);
    glUniform3f(viewPosLoc, 0.0f, 0.0f, 20.0f);
    glUniform1f(chargeLoc, static_cast<float>(sphere->charge));
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SphereRenderer::render(const Entity* entity, const float* view, const float* projection) {
    if (!entity || !shader_program) return;
    
    glUseProgram(shader_program);
    
    float model[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        static_cast<float>(entity->position.x),
        static_cast<float>(entity->position.y),
        static_cast<float>(entity->position.z),
        1
    };
    
    float color[3];
    if (entity->charge > 0) {
        color[0] = 1.0f; color[1] = 0.3f; color[2] = 0.3f;
    } else if (entity->charge < 0) {
        color[0] = 0.3f; color[1] = 0.3f; color[2] = 1.0f;
    } else {
        if (strcmp(entity->name, "StarA") == 0) {
            color[0] = 1.0f; color[1] = 0.8f; color[2] = 0.2f;
        } else if (strcmp(entity->name, "StarB") == 0) {
            color[0] = 1.0f; color[1] = 0.4f; color[2] = 0.1f;
        } else {
            color[0] = 0.3f; color[1] = 1.0f; color[2] = 0.3f;
        }
    }
    
    GLuint modelLoc = glGetUniformLocation(shader_program, "model");
    GLuint viewLoc = glGetUniformLocation(shader_program, "view");
    GLuint projLoc = glGetUniformLocation(shader_program, "projection");
    GLuint colorLoc = glGetUniformLocation(shader_program, "objectColor");
    GLuint lightPosLoc = glGetUniformLocation(shader_program, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shader_program, "viewPos");
    GLuint chargeLoc = glGetUniformLocation(shader_program, "charge");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
    glUniform3fv(colorLoc, 1, color);
    glUniform3f(lightPosLoc, 50.0f, 50.0f, 50.0f);
    glUniform3f(viewPosLoc, 0.0f, 0.0f, 20.0f);
    glUniform1f(chargeLoc, static_cast<float>(entity->charge));
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

extern "C" {

SphereRenderer* create_sphere_renderer(float radius, unsigned int sectors, unsigned int stacks) {
    SphereRenderer* renderer = new SphereRenderer();
    if (!renderer->init(radius, sectors, stacks)) {
        delete renderer;
        return nullptr;
    }
    return renderer;
}

void destroy_sphere_renderer(SphereRenderer* renderer) {
    if (renderer) {
        delete renderer;
    }
}

void render_sphere(SphereRenderer* renderer, const Entity* entity) {
    if (renderer && entity) {
        float view[16], proj[16];
        GLint viewLoc, projLoc;
        
        glUseProgram(renderer->shader_program);
        viewLoc = glGetUniformLocation(renderer->shader_program, "view");
        projLoc = glGetUniformLocation(renderer->shader_program, "projection");
        
        glGetUniformfv(renderer->shader_program, viewLoc, view);
        glGetUniformfv(renderer->shader_program, projLoc, proj);
        
        renderer->render(entity, view, proj);
    }
}

}
