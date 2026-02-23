#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "include/cphysics.h"
#include "graphics/OpenGL/gl_sphere.hpp"
#include "include/graphics/OpenGL/gl_skybox.h"
#include "include/graphics/OpenGL/gl_camera.h"
#include "include/core/entity_manager.h"

#include <stdio.h>
#include <stdlib.h>

#define PHYSICS_SCALE 1e-9   // 调整物理尺度，使行星运动可见
#define RENDER_SCALE 1e-9    // 渲染尺度，与物理尺度匹配

// 物理场定义
gravitational_field gravity_field = {
    .magnitude = 9.8,      // 地球重力加速度
    .direction = {0.0, -1.0, 0.0}
};

electric_field e_field = {
    .magnitude = 5.0,      // 电场强度
    .direction = {1.0, 0.0, 0.0}
};

magnetic_field b_field = {
    .magnitude = 2.0,      // 磁场强度
    .direction = {0.0, 0.0, 1.0},
    .position = {0.0, 0.0, 0.0}
};


static void update_physics_with_manager(EntityManager& manager, double dt) {
    // 先应用物理场（在加速度重置之前）
    manager.applyGravityField(9.8, {0.0, -1.0, 0.0});
    manager.applyElectricField(5.0, {1.0, 0.0, 0.0});
    manager.applyMagneticField(2.0, {0.0, 0.0, 1.0});
    
    // 然后进行物理更新（包括加速度重置和运动计算）
    manager.updateAllPhysics(dt);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "cPhysics Demo - Charged Particles", NULL, NULL);
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
    camera_init(&camera, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f); // 调整相机位置，使其能看到两个球体
    camera_set_global(&camera);
    float view[16];
    camera_update_view_matrix(view, &camera);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera_mouse_callback);

    // 使用EntityManager管理实体
    EntityManager entity_manager;
    
    Vector zero = {0, 0, 0};
    
    // 使用EntityManager添加实体
    entity_manager.addEntity("Sun", 1.989e30, 0.0, 
                           {0, 0, 0}, {0, 0, 0}, 
                           10.0 * PHYSICS_SCALE, true);
    
    entity_manager.addEntity("Planet", 5.972e24, 1.0,
                           {1.5e11 * PHYSICS_SCALE, 0, 0}, {0, 0, 3e4 * PHYSICS_SCALE},
                           1.0 * PHYSICS_SCALE, false);
    
    entity_manager.addEntity("NegCharge", 1e22, -1.0,
                           {-1e11 * PHYSICS_SCALE, 0, 0}, {0, 2e4 * PHYSICS_SCALE, 0},
                           0.5 * PHYSICS_SCALE, false);
    
    entity_manager.addEntity("Neutral", 1e22, 0.0,
                           {0, 1e11 * PHYSICS_SCALE, 0}, {2e4 * PHYSICS_SCALE, 0, 0},
                           0.5 * PHYSICS_SCALE, false);
    
    // 可以动态添加更多实体
    entity_manager.addEntity("Extra", 1e21, 0.5,
                           {0, -1e11 * PHYSICS_SCALE, 0}, {1e4 * PHYSICS_SCALE, 0, 0},
                           0.3 * PHYSICS_SCALE, false);

    double last_time = glfwGetTime();
    double physics_dt = 0.01; // 物理时间步长（秒），更小的值更精确但更耗性能
    double time_scale = 3600.0; // 时间缩放因子：1帧 = 1小时物理时间
    double accumulator = 0.0;
    int paused = 0;

    printf("=== cPhysics Demo - EntityManager Enhanced Physics ===\n");
    printf("Using EntityManager with vector-based entity management\n");
    printf("Entity count: %zu\n", entity_manager.getEntityCount());
    printf("\nSimulating multiple bodies with different physical properties:\n");
    
    // 显示所有实体的信息
    size_t index = 0;
    for (Entity* entity : entity_manager) {
        const char* color_desc = "";
        if (entity->charge > 0) color_desc = "(red)";
        else if (entity->charge < 0) color_desc = "(blue)";
        else color_desc = "(green)";
        
        printf("  %s %s: Mass=%.1e, Charge=%.1f\n", 
               entity->name, color_desc, entity->mass, entity->charge);
        index++;
    }
    
    printf("\nApplied Physics Fields:\n");
    printf("  Gravity: 9.8 m/s² downward\n");
    printf("  Electric Field: 5.0 N/C rightward\n");
    printf("  Magnetic Field: 2.0 T upward\n");
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
            
            // 使用EntityManager重置所有实体
            entity_manager.clearAll();
            
            // 重新添加实体
            entity_manager.addEntity("Sun", 1.989e30, 0.0, 
                                   {0, 0, 0}, {0, 0, 0}, 
                                   10.0 * PHYSICS_SCALE, true);
            
            entity_manager.addEntity("Planet", 5.972e24, 1.0,
                                   {1.5e11 * PHYSICS_SCALE, 0, 0}, {0, 0, 3e4 * PHYSICS_SCALE},
                                   1.0 * PHYSICS_SCALE, false);
            
            entity_manager.addEntity("NegCharge", 1e22, -1.0,
                                   {-1e11 * PHYSICS_SCALE, 0, 0}, {0, 2e4 * PHYSICS_SCALE, 0},
                                   0.5 * PHYSICS_SCALE, false);
            
            entity_manager.addEntity("Neutral", 1e22, 0.0,
                                   {0, 1e11 * PHYSICS_SCALE, 0}, {2e4 * PHYSICS_SCALE, 0, 0},
                                   0.5 * PHYSICS_SCALE, false);
            
            entity_manager.addEntity("Extra", 1e21, 0.5,
                                   {0, -1e11 * PHYSICS_SCALE, 0}, {1e4 * PHYSICS_SCALE, 0, 0},
                                   0.3 * PHYSICS_SCALE, false);
            
            printf("[RESET] Entity count: %zu\n", entity_manager.getEntityCount());
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
            r_pressed = 0;
        }
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (!paused) {
            accumulator += frame_time * time_scale;  // 应用时间缩放
            while (accumulator >= physics_dt) {
                update_physics_with_manager(entity_manager, physics_dt);
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
    destroy_sphere_renderer(renderer);
    glfwTerminate();
    return 0;
}
