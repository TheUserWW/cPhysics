#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "include/cphysics.h"
#include "graphics/OpenGL/gl_sphere.hpp"
#include "include/graphics/OpenGL/gl_skybox.h"
#include "include/graphics/OpenGL/gl_camera.h"
#include "include/core/entity_manager.h"
#include "include/graphics/OpenGL/gl_model.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PHYSICS_SCALE 1e-9
#define RENDER_SCALE 1e-9

static void update_physics_binary(EntityManager& manager, double dt) {
    manager.updateAllPhysics(dt);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "cPhysics Demo - Object Collision", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    SphereRenderer* renderer = create_sphere_renderer(1.0f, 16, 16);
    if (!renderer)
    {
        printf("Failed to create sphere renderer\n");
        glfwTerminate();
        return -1;
    }

    SkyboxRenderer* skybox = create_skybox_renderer("resources/skybox/NightSkyHDRI009_8K_HDR.hdr");
    if (!skybox)
    {
        printf("Warning: Failed to create skybox renderer\n");
    }

    Model* appleModel = new Model("resources/models/Apple.obj");
    if (appleModel->loadModel()) {
        printf("Successfully loaded Apple model\n");
    } else {
        printf("Failed to load Apple model\n");
        delete appleModel;
        appleModel = nullptr;
    }

    float aspect = 1920.0f / 1080.0f;
    float fov = 45.0f;
    float near = 0.1f;
    float far = 1000.0f;
    float f = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    float projection[16] = {
        f / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (far + near) / (near - far), -1.0f,
        0.0f, 0.0f, (2.0f * far * near) / (near - far), 0.0f
    };

    Camera camera;
    camera_init(&camera, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f);
    camera_set_global(&camera);
    float view[16];
    camera_update_view_matrix(view, &camera);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera_mouse_callback);

    EntityManager entity_manager;
    
    double M1 = 1.0;
    double M2 = 1.0;
    double separation = 10.0;
    double collision_speed = 5.0;
    
    printf("Object Collision Demo Parameters:\n");
    printf("  Object A: Mass = %.2e kg, Position = %.2e m\n", M1, separation / 2.0);
    printf("  Object B: Mass = %.2e kg, Position = %.2e m\n", M2, -separation / 2.0);
    printf("  Initial separation: %.2e m\n", separation);
    printf("  Collision speed: %.2e m/s\n", collision_speed);
    
    entity_manager.addEntity("ObjectA", M1, 0.8, 
                           {separation / 2.0, 0, 0}, {-collision_speed, 0, 0},
                           1.0, false);
    
    entity_manager.addEntity("ObjectB", M2, 0.8, 
                           {-separation / 2.0, 0, 0}, {collision_speed, 0, 0},
                           1.0, false);

    double last_time = glfwGetTime();
    double physics_dt = 0.001;
    double accumulator = 0.0;
    int paused = 0;

    printf("\n=== cPhysics Demo - Object Collision ===\n");
    printf("Simulating two objects colliding with each other\n");
    printf("Entity count: %zu\n", entity_manager.getEntityCount());
    
    for (Entity* entity : entity_manager) {
        printf("  %s: Mass=%.2e kg, Radius=%.1f\n", 
               entity->name, entity->mass, entity->coefficient_of_restitution);
    }
    
    printf("\nControls:\n");
    printf("  WASD     - Move camera\n");
    printf("  Q/E      - Move up/down\n");
    printf("  Mouse    - Look around\n");
    printf("  SPACE    - Pause/Resume\n");
    printf("  R        - Reset simulation\n");
    printf("  ESC      - Exit\n");

    while (!glfwWindowShouldClose(window))
    {
        double current_time = glfwGetTime();
        double frame_time = current_time - last_time;
        last_time = current_time;
        
        if (frame_time > 0.1) frame_time = 0.1;

        camera_process_input(window, &camera, frame_time);
        
        static int space_pressed = 0;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !space_pressed) {
            paused = !paused;
            space_pressed = 1;
            printf(paused ? "[PAUSED]\n" : "[RESUMED]\n");
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            space_pressed = 0;
        }
        
        static int r_pressed = 0;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !r_pressed) {
            r_pressed = 1;
            
            entity_manager.clearAll();
            
            double M1_reset = 1.0;
            double M2_reset = 1.0;
            double separation_reset = 10.0;
            double collision_speed_reset = 5.0;
            
            entity_manager.addEntity("ObjectA", M1_reset, 0.8, 
                                   {separation_reset / 2.0, 0, 0}, {-collision_speed_reset, 0, 0},
                                   1.0, false);
            
            entity_manager.addEntity("ObjectB", M2_reset, 0.8, 
                                   {-separation_reset / 2.0, 0, 0}, {collision_speed_reset, 0, 0},
                                   1.0, false);
            
            printf("[RESET] Collision simulation restored\n");
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
            r_pressed = 0;
        }
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (!paused) {
            accumulator += frame_time;
            while (accumulator >= physics_dt) {
                update_physics_binary(entity_manager, physics_dt);
                accumulator -= physics_dt;
            }
        }

        camera_update_view_matrix(view, &camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

        if (skybox) {
            render_skybox(skybox, view, projection);
        }

        if (appleModel) {
            float modelMatrix[16] = {
                0.1f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.1f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.1f, 0.0f,
                5.0f, 0.0f, 0.0f, 1.0f
            };
            float appleColor[3] = {1.0f, 0.3f, 0.2f};
            appleModel->render(view, projection, modelMatrix, appleColor);
        }

        GLuint proj_loc = glGetUniformLocation(renderer->shader_program, "projection");
        GLuint view_loc = glGetUniformLocation(renderer->shader_program, "view");
        glUseProgram(renderer->shader_program);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection);
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);

        // 使用EntityManager的迭代器渲染所有实体
        for (Entity* entity : entity_manager) {
            render_sphere(renderer, entity);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if (skybox) {
        destroy_skybox_renderer(skybox);
    }
    if (appleModel) {
        appleModel->cleanup();
        delete appleModel;
    }
    destroy_sphere_renderer(renderer);
    glfwTerminate();
    return 0;
}
