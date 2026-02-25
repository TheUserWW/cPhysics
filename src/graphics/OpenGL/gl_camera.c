//
// Created by wcx16 on 2026/2/22.
//

#include "graphics/OpenGL/gl_camera.h"
#include <math.h>
#include <stdio.h>

static Camera* g_camera = NULL;

void camera_init(Camera* cam, float x, float y, float z, float yaw, float pitch) {
    cam->position.x = x;
    cam->position.y = y;
    cam->position.z = z;
    cam->yaw = yaw;
    cam->pitch = pitch;
    cam->first_mouse = 1;
    cam->last_mouse_x = 0.0f;
    cam->last_mouse_y = 0.0f;
    cam->velocity.x = 0.0;
    cam->velocity.y = 0.0;
    cam->velocity.z = 0.0;
    cam->acceleration = 50.0f;
}

void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!g_camera) return;
    
    if (g_camera->first_mouse) {
        g_camera->last_mouse_x = (float)xpos;
        g_camera->last_mouse_y = (float)ypos;
        g_camera->first_mouse = 0;
        return;
    }
    
    float xoffset = (float)xpos - g_camera->last_mouse_x;
    float yoffset = g_camera->last_mouse_y - (float)ypos;
    g_camera->last_mouse_x = (float)xpos;
    g_camera->last_mouse_y = (float)ypos;
    
    float sensitivity = 0.002f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    g_camera->yaw += xoffset;
    g_camera->pitch += yoffset;
    
    if (g_camera->pitch > 1.5f) g_camera->pitch = 1.5f;
    if (g_camera->pitch < -1.5f) g_camera->pitch = -1.5f;
}

void camera_update_view_matrix(float* view, const Camera* cam) {
    float cos_yaw = cosf(cam->yaw);
    float sin_yaw = sinf(cam->yaw);
    float cos_pitch = cosf(cam->pitch);
    float sin_pitch = sinf(cam->pitch);

    Vector forward = {
        sin_yaw * cos_pitch,
        sin_pitch,
        -cos_yaw * cos_pitch
    };
    Vector right = {
        cos_yaw,
        0.0f,
        sin_yaw
    };
    Vector up = cross_product(right, forward);

    view[0] = right.x; view[1] = up.x; view[2] = -forward.x; view[3] = 0.0f;
    view[4] = right.y; view[5] = up.y; view[6] = -forward.y; view[7] = 0.0f;
    view[8] = right.z; view[9] = up.z; view[10] = -forward.z; view[11] = 0.0f;
    view[12] = -(right.x*cam->position.x + right.y*cam->position.y + right.z*cam->position.z);
    view[13] = -(up.x*cam->position.x + up.y*cam->position.y + up.z*cam->position.z);
    view[14] = forward.x*cam->position.x + forward.y*cam->position.y + forward.z*cam->position.z;
    view[15] = 1.0f;
}

void camera_process_input(GLFWwindow* window, Camera* cam, double delta_time) {
    float cos_yaw = cosf(cam->yaw);
    float sin_yaw = sinf(cam->yaw);
    float cos_pitch = cosf(cam->pitch);
    float sin_pitch = sinf(cam->pitch);
    float dt = (float)delta_time;
    float acc = cam->acceleration;
    float damping = 0.95f;

    int w_pressed = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    int s_pressed = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    int a_pressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    int d_pressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    int q_pressed = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    int e_pressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    if (w_pressed) {
        cam->velocity.x += sin_yaw * cos_pitch * acc * dt;
        cam->velocity.y += sin_pitch * acc * dt;
        cam->velocity.z -= cos_yaw * cos_pitch * acc * dt;
    }
    if (s_pressed) {
        cam->velocity.x -= sin_yaw * cos_pitch * acc * dt;
        cam->velocity.y -= sin_pitch * acc * dt;
        cam->velocity.z += cos_yaw * cos_pitch * acc * dt;
    }
    if (a_pressed) {
        cam->velocity.x -= cos_yaw * acc * dt;
        cam->velocity.z -= sin_yaw * acc * dt;
    }
    if (d_pressed) {
        cam->velocity.x += cos_yaw * acc * dt;
        cam->velocity.z += sin_yaw * acc * dt;
    }
    if (q_pressed) {
        cam->velocity.y -= acc * dt;
    }
    if (e_pressed) {
        cam->velocity.y += acc * dt;
    }

    if (!w_pressed && !s_pressed && !a_pressed && !d_pressed) {
        cam->velocity.x *= damping;
        cam->velocity.z *= damping;
    }
    if (!w_pressed && !s_pressed && !q_pressed && !e_pressed) {
        cam->velocity.y *= damping;
    }

    cam->position.x += cam->velocity.x * dt;
    cam->position.y += cam->velocity.y * dt;
    cam->position.z += cam->velocity.z * dt;
}

void camera_set_global(Camera* cam) {
    g_camera = cam;
}

Camera* camera_get_global(void) {
    return g_camera;
}
#