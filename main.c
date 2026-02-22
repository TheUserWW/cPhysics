#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "include/cphysics.h"
#include "graphics/OpenGL/gl_sphere.h"
#include "include/graphics/OpenGL/gl_skybox.h"
#include "include/graphics/OpenGL/gl_camera.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_ENTITIES 10
#define PHYSICS_SCALE 0.01



static void update_physics(Entity* entities, int count, double dt) {
    for (int i = 0; i < count; i++) {
        if (entities[i].is_static) continue;
        
        entities[i].acceleration = (Vector){0, 0, 0};
    }
    
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            apply_electric_force(&entities[i], &entities[j]);
            apply_electric_force(&entities[j], &entities[i]);
        }
    }
    
    for (int i = 0; i < count; i++) {
        if (entities[i].is_static) continue;
        
        entities[i].velocity.x += entities[i].acceleration.x * dt;
        entities[i].velocity.y += entities[i].acceleration.y * dt;
        entities[i].velocity.z += entities[i].acceleration.z * dt;
        
        entities[i].position.x += entities[i].velocity.x * dt;
        entities[i].position.y += entities[i].velocity.y * dt;
        entities[i].position.z += entities[i].velocity.z * dt;
    }
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "cPhysics Demo - Charged Particles", NULL, NULL);
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

    SphereRenderer* renderer = create_sphere_renderer(0.3f, 16, 16);
    if (!renderer)
    {
        printf("Failed to create sphere renderer\n");
        glfwTerminate();
        return -1;
    }

    SkyboxRenderer* skybox = create_skybox_renderer("resources/skybox/golden_gate_hills_4k.hdr");
    if (!skybox)
    {
        printf("Warning: Failed to create skybox renderer\n");
    }

    float aspect = 800.0f / 600.0f;
    float fov = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    float f = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    float projection[16] = {
        f / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (far + near) / (near - far), -1.0f,
        0.0f, 0.0f, (2.0f * far * near) / (near - far), 0.0f
    };

    Camera camera;
    camera_init(&camera, 0.0f, 2.0f, 8.0f, 0.0f, -0.2f);
    camera_set_global(&camera);
    float view[16];
    camera_update_view_matrix(view, &camera);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera_mouse_callback);

    Entity entities[MAX_ENTITIES];
    int entity_count = 6;
    
    Vector zero = {0, 0, 0};
    entities[0] = new_entity("center_positive", 1.0, 1e-6, &zero, &zero, &zero, 0.8, true, true);
    entities[0].position = (Vector){0, 0, 0};
    
    Vector v1 = {2.0, 0.5, 0.3};
    entities[1] = new_entity("negative_1", 0.1, -1e-7, &zero, &v1, &zero, 0.8, false, false);
    entities[1].position = (Vector){2, 0, 0};
    
    Vector v2 = {-1.5, 0.3, 0.5};
    entities[2] = new_entity("negative_2", 0.1, -1e-7, &zero, &v2, &zero, 0.8, false, false);
    entities[2].position = (Vector){-2, 0, 0};
    
    Vector v3 = {0.2, 0.4, 1.8};
    entities[3] = new_entity("negative_3", 0.1, -1e-7, &zero, &v3, &zero, 0.8, false, false);
    entities[3].position = (Vector){0, 0, 2};
    
    Vector v4 = {-0.3, -0.2, -1.5};
    entities[4] = new_entity("negative_4", 0.1, -1e-7, &zero, &v4, &zero, 0.8, false, false);
    entities[4].position = (Vector){0, 0, -2};
    
    Vector v5 = {0.5, 1.0, 0.2};
    entities[5] = new_entity("negative_5", 0.1, -1e-7, &zero, &v5, &zero, 0.8, false, false);
    entities[5].position = (Vector){0, 2, 0};

    double last_time = glfwGetTime();
    double physics_dt = 0.001;
    double accumulator = 0.0;
    int paused = 0;

    printf("=== cPhysics Demo ===\n");
    printf("Controls:\n");
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
            Vector zero_vec = {0, 0, 0};
            
            entities[0].position = (Vector){0, 0, 0};
            entities[0].velocity = zero_vec;
            
            Vector v1_reset = {2.0, 0.5, 0.3};
            entities[1].position = (Vector){2, 0, 0};
            entities[1].velocity = v1_reset;
            
            Vector v2_reset = {-1.5, 0.3, 0.5};
            entities[2].position = (Vector){-2, 0, 0};
            entities[2].velocity = v2_reset;
            
            Vector v3_reset = {0.2, 0.4, 1.8};
            entities[3].position = (Vector){0, 0, 2};
            entities[3].velocity = v3_reset;
            
            Vector v4_reset = {-0.3, -0.2, -1.5};
            entities[4].position = (Vector){0, 0, -2};
            entities[4].velocity = v4_reset;
            
            Vector v5_reset = {0.5, 1.0, 0.2};
            entities[5].position = (Vector){0, 2, 0};
            entities[5].velocity = v5_reset;
            
            printf("[RESET]\n");
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
                update_physics(entities, entity_count, physics_dt);
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

        GLuint proj_loc = glGetUniformLocation(renderer->shader_program, "projection");
        GLuint view_loc = glGetUniformLocation(renderer->shader_program, "view");
        glUseProgram(renderer->shader_program);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection);
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);

        for (int i = 0; i < entity_count; i++) {
            render_sphere(renderer, &entities[i]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if (skybox) {
        destroy_skybox_renderer(skybox);
    }
    destroy_sphere_renderer(renderer);
    glfwTerminate();
    return 0;
}
