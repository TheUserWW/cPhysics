//
// Created by wcx16 on 2026/2/22.
//

#include "graphics/OpenGL/gl_camera.h"
#include <math.h>
#include <stdio.h>

static Camera* g_camera = NULL;

void camera_init(Camera* cam, float x, float y, float z, float yaw, float pitch) {
    cam->x = x;
    cam->y = y;
    cam->z = z;
    cam->yaw = yaw;
    cam->pitch = pitch;
    cam->first_mouse = 1;
    cam->last_mouse_x = 0.0f;
    cam->last_mouse_y = 0.0f;
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

    float forward[3] = {
        sin_yaw * cos_pitch,
        sin_pitch,
        -cos_yaw * cos_pitch
    };
    float right[3] = {
        cos_yaw,
        0.0f,
        sin_yaw
    };
    float up[3] = {
        right[1] * forward[2] - right[2] * forward[1],
        right[2] * forward[0] - right[0] * forward[2],
        right[0] * forward[1] - right[1] * forward[0]
    };

    view[0] = right[0]; view[1] = up[0]; view[2] = -forward[0]; view[3] = 0.0f;
    view[4] = right[1]; view[5] = up[1]; view[6] = -forward[1]; view[7] = 0.0f;
    view[8] = right[2]; view[9] = up[2]; view[10] = -forward[2]; view[11] = 0.0f;
    view[12] = -(right[0]*cam->x + right[1]*cam->y + right[2]*cam->z);
    view[13] = -(up[0]*cam->x + up[1]*cam->y + up[2]*cam->z);
    view[14] = forward[0]*cam->x + forward[1]*cam->y + forward[2]*cam->z;
    view[15] = 1.0f;
}

void camera_process_input(GLFWwindow* window, Camera* cam, double delta_time) {
    float cos_yaw = cosf(cam->yaw);
    float sin_yaw = sinf(cam->yaw);
    float velocity = 2.5f * (float)delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam->x += sin_yaw * velocity;
        cam->z -= cos_yaw * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam->x -= sin_yaw * velocity;
        cam->z += cos_yaw * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam->x -= cos_yaw * velocity;
        cam->z -= sin_yaw * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam->x += cos_yaw * velocity;
        cam->z += sin_yaw * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cam->y -= velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cam->y += velocity;
    }
}

void camera_set_global(Camera* cam) {
    g_camera = cam;
}

Camera* camera_get_global(void) {
    return g_camera;
}
#