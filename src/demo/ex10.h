#ifndef EX10_1_H
#define EX10_1_H

#include "chcommon.h"
#include "camera.h"

namespace ex10
{
    static f32 delta_time = 0;
    Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        static f32 mouse_last_x = 400;
        static f32 mouse_last_y = 300;
        static bool first_mouse = true;

        if (first_mouse)
        {
            mouse_last_x = xpos;
            mouse_last_y = ypos;
            first_mouse = false;
        }

        f32 xoffset = xpos - mouse_last_x;
        f32 yoffset = mouse_last_y - ypos;
        mouse_last_x = xpos;
        mouse_last_y = ypos;

        cam.ProcessMouseMovement(xoffset, yoffset);
    };

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        cam.ProcessMouseScroll(yoffset);
    }

    void Start1(GLFWwindow* window)
    {
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetScrollCallback(window, scroll_callback);
        cam.isFPS = true;
    }

    glm::mat4 LookAt(const glm::vec3& src, const glm::vec3& dest, const glm::vec3& up)
    {
        glm::vec3 front = dest - src;
        front = glm::normalize(front);
        glm::vec3 right = glm::normalize(glm::cross(front, up));
        glm::vec3 new_up = glm::normalize(glm::cross(right, front));
        glm::mat4 view(
            glm::vec4(right, 0.0f),
            glm::vec4(new_up, 0.0f),
            glm::vec4(-front, 0.0f),
            glm::vec4(src, 1.0f)
        );

        return glm::inverse(view);
    }

    void Run1(ch_return input, f32 aspect_ratio, f32 dt)
    {
        delta_time = dt;
        glm::mat4 tmpview = LookAt(cam.position_, cam.position_ + cam.lookat_, cam.up_);
        f32 fov = cam.zoom_;
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
        input.shader.Use();
        input.shader.SetMat4f("view", tmpview);
        input.shader.SetMat4f("projection", projection);
        input.shader.Unbind();
        GLQueryError();
        
        ch9::Run(input);
    }

    void Ex10ProcessInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.ProcessKeyboard(CameraMovement::kForward, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.ProcessKeyboard(CameraMovement::kBackward, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.ProcessKeyboard(CameraMovement::kLeft, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.ProcessKeyboard(CameraMovement::kRight, delta_time);
    }

} // namespace ex10

#endif // EX10_1_H