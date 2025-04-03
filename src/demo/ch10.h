#pragma once

#include "chcommon.h"
#include "ch9.h"

/*
Chapter 10 Camera

Defining a camera requires a few things
1. Position
2. Direction (lookat)
3. A vector poiting to its right
4. A vector pointing up from the camera
Note: This defines a vector space from the camera's perspective
We are essentially defining the origin, and basis vectors.

We can then use these vectors, and the origin to define a [lookat matrix]
[Rx Ry Rz 0]   [1 0 0 -Px]   
[Ux Uy Uz 0]   [0 1 0 -Py]
[Dx Dy Dz 0] * [0 0 1 -Pz]
[0  0  0  1]   [0 0 0  1 ]

Note: that the basis matrix for the camera has been transposed.
We don't need to do the drudge work ourselves because glm already does that for us
with:
glm::lookAt(pos, target, up)
*/

namespace ch10
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Define the bases for the camera's space
    // glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 camera_forward = glm::normalize(camera_pos - camera_target);
    // glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_forward));
    // glm::vec3 camera_up = glm::normalize(glm::cross(camera_forward, camera_right));

    // Walking around
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = up;

    static f64 delta_time = 0.0f;
    void Run(ch_return input, f64 dt)
    {
        delta_time = dt;
        // Define a lookat matrix
        const float kRadius = 20.0f;
        f32 cam_x_pos = sin(glfwGetTime()) * kRadius;
        f32 cam_z_pos = cos(glfwGetTime()) * kRadius;
        
        glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

        input.shader.Use();
        input.shader.SetMat4f("view", view);
        input.shader.Unbind();
        GLQueryError();
        ch9::Run(input);
    }

    void Ch10ProcessInput(GLFWwindow* window)
    {
        const f32 kCameraSpeed = 1.0f * delta_time;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera_pos += kCameraSpeed * camera_front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera_pos -= kCameraSpeed * camera_front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * kCameraSpeed; // << Move to the left
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * kCameraSpeed; // << Move to the left
    }
}

