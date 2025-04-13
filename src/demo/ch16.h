#ifndef CH16
#define CH16

#include "chcommon.h"

namespace ch16
{
    static f32 aspect_ratio = 1.0f;
    static f32 delta_time = 0.0f;
    Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
    u32 container_vao;
    u32 light_vao;
    u32 vbo;
    
    // Define the GLFW callbacks
    DECL_GLFW_CURSOR_CALLBACK(ch16_cursor_callback)
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
    
    DECL_GLFW_SCROLL_CALLBACK(ch16_scroll_callback)
    {
        cam.ProcessMouseScroll(yoffset);
    }
    
    DECL_GLFW_MOVEMENT_CALLBACK(ch16_movement_callback)
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

    lighting_return Start
    (
        f32 ar, 
        const std::string& vpath_container, const std::string& vpath_light,
        const std::string& fpath_container, const std::string& fpath_light
    )
    {
        aspect_ratio = ar;
        GLFWCursorCallback = ch16_cursor_callback;
        GLFWScrollCallback = ch16_scroll_callback;
        GLFWMovementCallback = ch16_movement_callback;
        cam.movement_speed_ = 3.0f;

        glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glGenVertexArrays(1, &container_vao);
        glGenVertexArrays(1, &light_vao);

        glBindVertexArray(container_vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kTheCube), kTheCube, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3*sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(light_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        Shader container_shader(vpath_container.c_str(), fpath_container.c_str());
        Shader light_shader(vpath_light.c_str(), fpath_light.c_str());
        glm::vec3 light_dir(0.5f, 1.0f, 0.2f);
        // glm::vec3 light_pos = -20.0f * light_dir; // << directional light
        glm::vec3 light_pos = -4.0f * light_dir;
        

        glm::mat4 container_model(1.0f);
        container_shader.Use();
        container_shader.SetMat4f("model", container_model);
        container_shader.SetMat4f("view", cam.GetViewMatrix());
        container_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        container_shader.SetVec3f("light.position", cam.position_); // << spot light
        container_shader.SetVec3f("light.direction", cam.lookat_); // << spot light
        container_shader.SetFloat("light.cutoff", glm::cos(glm::radians(12.5f))); // << spot light
        container_shader.SetFloat("light.outer_cutoff", glm::cos(glm::radians(17.5f))); // << spot light
        // container_shader.SetVec3f("light.position", light_pos); // << point light
        container_shader.SetFloat("light.constant",  1.0f);
        container_shader.SetFloat("light.linear",    0.09f);
        container_shader.SetFloat("light.quadratic", 0.032f);
        // container_shader.SetVec3f("light.direction", light_dir); // << directional light
        container_shader.SetVec3f("light.ambient", glm::vec3(0.02f));
        container_shader.SetVec3f("light.diffuse", glm::vec3(0.5f));
        container_shader.SetVec3f("light.specular", glm::vec3(1.0f));
        container_shader.SetVec3f("material.specular", 0.5f, 0.5f, 0.5f);
        container_shader.SetFloat("material.shininess", 32.0f);
        container_shader.Unbind();
        Texture container_diffuse_map("../images/container2.png", GL_RGBA);
        Texture container_specular_map("../images/container2_specular.png", GL_RGBA);
        // Texture container_emission_map("../images/matrix.jpg", GL_RGB);
        // Set Wrapping modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set Filtering modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Activate texture units
        container_diffuse_map.Activate(0, container_shader, "material.diffuse");
        container_specular_map.Activate(1, container_shader, "material.specular");
        // container_emission_map.Activate(2, container_shader, "material.emission");

        glm::mat4 light_model(1.0f);
        light_model = glm::translate(light_model, light_pos);
        light_model = glm::scale(light_model, glm::vec3(0.2f));
        light_shader.Use();
        light_shader.SetMat4f("model", light_model);
        light_shader.SetMat4f("view", cam.GetViewMatrix());
        light_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        light_shader.Unbind();
        
        return {light_shader, container_shader};
    }
    
    std::vector<glm::vec3> container_positions = 
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    void Run(lighting_return input, f32 dt)
    {
        delta_time = dt;
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update the lighting parameters with cam data
        input.object_shader.Use();
        input.object_shader.SetVec3f("light.position", cam.position_); // << spot light
        input.object_shader.SetVec3f("light.direction", cam.lookat_); // << spot light
        input.object_shader.Unbind();

        for (int i = 0; i < container_positions.size(); i++)
        {
            glm::vec3 pos = container_positions[i];
            glm::mat4 model(1.0f);
            model = glm::translate(model, pos);
            f32 angle = 20.0f * i * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            glBindVertexArray(container_vao);
            input.object_shader.Use();
            input.object_shader.SetMat4f("model", model);
            input.object_shader.SetMat4f("view", cam.GetViewMatrix());
            input.object_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
            input.object_shader.SetVec3f("view_pos", cam.position_);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            input.object_shader.Unbind();
            glBindVertexArray(0);
        }
        

        glBindVertexArray(light_vao);
        input.light_shader.Use();
        input.light_shader.SetMat4f("view", cam.GetViewMatrix());
        input.light_shader.SetMat4f("projection", glm::perspective(glm::radians(cam.zoom_), aspect_ratio, 0.1f, 100.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        input.light_shader.Unbind();
        glBindVertexArray(0);
    }

} // namespace ch16
#endif // CH16