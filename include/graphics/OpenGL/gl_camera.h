#ifndef CPHYSICS_GL_CAMERA_H
#define CPHYSICS_GL_CAMERA_H

#include <GLFW/glfw3.h>

typedef struct {
    float x, y, z;
    float yaw;
    float pitch;
    int first_mouse;
    float last_mouse_x;
    float last_mouse_y;
} Camera;

// Initialize camera
void camera_init(Camera* cam, float x, float y, float z, float yaw, float pitch);

// Mouse callback function
void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Update view matrix
void camera_update_view_matrix(float* view, const Camera* cam);

// Process keyboard input to control camera movement
void camera_process_input(GLFWwindow* window, Camera* cam, double delta_time);

// Set global camera pointer
void camera_set_global(Camera* cam);

// Get global camera pointer
Camera* camera_get_global(void);

#endif //CPHYSICS_GL_CAMERA_H