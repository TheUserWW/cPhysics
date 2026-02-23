#ifndef CPHYSICS_GL_SPHERE_HPP
#define CPHYSICS_GL_SPHERE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "core/entity.h"
#include <vector>
#include <string>

class Sphere : public Entity {
public:
    double radius;
    float color[3];

    Sphere();
    Sphere(const char* name, double mass, double charge, 
           const Vector& position, const Vector& velocity,
           double radius, const float color[3] = nullptr);
    
    void setRadius(double r) { radius = r; }
    double getRadius() const { return radius; }
    
    void setColor(float r, float g, float b);
    void getColor(float* outColor) const;
    
    double getVolume() const;
    double getDensity() const;
    double getSurfaceArea() const;
};

class SphereRenderer {
public:
    GLuint shader_program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    unsigned int indexCount;
    float baseRadius;

    SphereRenderer();
    ~SphereRenderer();

    bool init(float radius = 1.0f, unsigned int sectors = 16, unsigned int stacks = 16);
    void cleanup();

    void render(const Sphere* sphere, const float* view, const float* projection);
    void render(const Entity* entity, const float* view, const float* projection);
};

#ifdef __cplusplus
extern "C" {
#endif

SphereRenderer* create_sphere_renderer(float radius, unsigned int sectors, unsigned int stacks);
void destroy_sphere_renderer(SphereRenderer* renderer);
void render_sphere(SphereRenderer* renderer, const Entity* entity);

#ifdef __cplusplus
}
#endif

#endif
