#ifndef CH10
#define CH10

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

    static f32 pitch = 0.0f;
    static f32 yaw = -90.0f; // Look in the -z direction

    static f64 delta_time = 0.0f;

    // Set a mouse callback
    static f32 mouse_last_x = 400;
    static f32 mouse_last_y = 300;
    f32 xoffset = 0.0f;
    f32 yoffset = 0.0f;
    bool first_mouse = true;
    void MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        // avoids jumps from your mouse entering the screen from the edge
        if (first_mouse)
        {
            mouse_last_x = xpos;
            mouse_last_y = ypos;
            first_mouse = false;
        }

        // 1. Calculate the mouse's offset since the last frame.
        f32 xoffset = xpos - mouse_last_x;
        f32 yoffset = mouse_last_y - ypos; // reversed: y grow from top to bottom to when you go down ypos grows
        mouse_last_x = xpos;
        mouse_last_y = ypos;

        const f32 kSensitivity = 0.1f;
        xoffset *= kSensitivity;
        yoffset *= kSensitivity;
        
        // 2. Add the offset values to the camera's yaw and pitch values.
        yaw += xoffset;
        pitch += yoffset;

        // 3. Add some constraints to the minimuxm/maximum pitch values.
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    static f32 zoom = 45.0f;
    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void Start(GLFWwindow* window)
    {
        glfwSetCursorPosCallback(window, MouseCallback);                // << Set a mouse cursor callback
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    // << Disable cursor visibility
        glfwSetScrollCallback(window, ScrollCallback);                  // Set the scroll whell callback
        /*
            When handling mouse input for a flyin camera these are the steps we need to take:
            1. Calculate the mouse's offset since the last frame.
            2. Add the offset values to the camera's yaw and pitch values.
            3. Add some constraints to the minimuxm/maximum pitch values.
            4. Calculate the irection vector.
        */
    }

    void Run(ch_return input, f32 aspect_ratio, f64 dt)
    {
        delta_time = dt;
        // Define a lookat matrix
        // Define direction vector from euler angles
        // 4. Calculate the irection vector.
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        camera_front = glm::normalize(direction);
        glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        
        // Createa a projection matrix that allows us to zoom
        f32 fov = zoom;
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);

        input.shader.Use();
        input.shader.SetMat4f("view", view);
        input.shader.SetMat4f("projection", projection);
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
} // namespace ch10

#endif // CH10
