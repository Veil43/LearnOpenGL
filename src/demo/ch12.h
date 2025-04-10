#ifndef CH12
#define CH12

#include "chcommon.h"
#include "cube.h"

/*
When it comes to colors, we can define the color of an object based on the ammount of light it reflects in each channel.
So for:
glm::vec3 light_color(0.0f, 1.0f, 0.0f); // Green Light
glm::vec3 toy_color(1.0f, 0.5f, 0.31f); 

The reflected color will be light_color * toy_color (the component-wise product) which gives (0.0f, 0.5f, 0.0f);
So under a green light our toy will look greenish.
*/
namespace lighting
{
    u32 container_vao = 0;
    u32 light_vao = 0;
    u32 vbo;
    f32 delta_time = 0.0f;
    f32 aspect_ratio = 1.0f;
    Camera cam(glm::vec3(0.0f, 0.0f, 5.0f));

    // Define the GLFW callbacks
    DECL_GLFW_CURSOR_CALLBACK(lighting_cursor_callback)
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
    }

    DECL_GLFW_SCROLL_CALLBACK(lighting_scroll_callback)
    {
        cam.ProcessMouseScroll(yoffset);
    }

    DECL_GLFW_MOVEMENT_CALLBACK(lighting_movement_callback)
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

    lighting_return Start(f32 ar, const std::string& vpath, const std::string& fpath, const std::string& lpath) 
    {
        aspect_ratio = ar;
        GLFWCursorCallback = lighting_cursor_callback;
        GLFWScrollCallback = lighting_scroll_callback;
        GLFWMovementCallback = lighting_movement_callback;

        // cam.isFPS_ = true;

        // glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glGenVertexArrays(1, &container_vao);
        glGenVertexArrays(1, &light_vao);

        glBindVertexArray(container_vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kTheCube), kTheCube, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(light_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        glm::vec3 container_color(1.0f, 0.5f, 0.31f);
        glm::vec3 light_color(1.0f, 1.0f, 1.0f);

        Shader container_shader(vpath.c_str(), fpath.c_str());
        Shader light_shader(vpath.c_str(), lpath.c_str());

        glm::mat4 container_model(1.0f);
        glm::mat4 light_model(1.0f);
        glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
        light_model = glm::translate(light_model, light_pos);
        light_model = glm::scale(light_model, glm::vec3(0.2f));

        container_shader.Use();
        container_shader.SetMat4f("model", container_model);
        container_shader.SetMat4f("view", cam.GetViewMatrix());
        container_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        glUniform3fv(glGetUniformLocation(container_shader.ID_, "object_color"), 1, glm::value_ptr(container_color));
        glUniform3fv(glGetUniformLocation(container_shader.ID_, "light_color"), 1, glm::value_ptr(light_color));
        container_shader.SetVec3f("light_pos", light_pos);
        container_shader.Unbind();

        light_shader.Use();
        light_shader.SetMat4f("model", light_model);
        light_shader.SetMat4f("view", cam.GetViewMatrix());
        light_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        light_shader.Unbind();
        
        return {light_shader, container_shader};
    }

    void Run(lighting_return input, f32 dt)
    {
        delta_time = dt;
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(container_vao);
        input.object_shader.Use();
        input.object_shader.SetMat4f("view", cam.GetViewMatrix());
        input.object_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        input.object_shader.SetVec3f("view_pos", cam.position_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        input.object_shader.Unbind();

        glBindVertexArray(light_vao);
        input.light_shader.Use();
        input.light_shader.SetMat4f("view", cam.GetViewMatrix());
        input.light_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        input.light_shader.Unbind();
        glBindVertexArray(0);

        glBindVertexArray(0);
    }

} // namepsace lighting

#endif // CH12
