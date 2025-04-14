#ifndef CH17
#define CH17

#include "chcommon.h"
#include "cube.h"

namespace ch17
{
    static f32 s_aspect_ratio = 1.0f;
    static f32 s_delta_time = 0.0f;
    static Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
    struct return_type
    {
        std::vector<object_3d> containers;
        std::vector<Shader> container_shaders;
        object_3d light;
        Shader light_shader;
    };

    DECL_GLFW_CURSOR_CALLBACK(ch17_cursor_callback)
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
    
    DECL_GLFW_SCROLL_CALLBACK(ch17_scroll_callback)
    {
        cam.ProcessMouseScroll(yoffset);
    }
    
    DECL_GLFW_MOVEMENT_CALLBACK(ch17_movement_callback)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(CameraMovement::kForward, s_delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(CameraMovement::kBackward, s_delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(CameraMovement::kLeft, s_delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(CameraMovement::kRight, s_delta_time);
    }

    return_type Start(f32 ap)
    {
        s_aspect_ratio = ap;
        GLFWCursorCallback = ch17_cursor_callback;
        GLFWScrollCallback = ch17_scroll_callback;
        GLFWMovementCallback = ch17_movement_callback;
        cam.movement_speed_ = 3.0f;
        glEnable(GL_DEPTH_TEST);

        glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        object_3d container_obj((vertex*)CubePCNU, 36);
        Shader container_shader("../src/demo/shaders/ch17/container.vs", "../src/demo/shaders/ch17/container.fs");
        container_shader.Use();
        container_shader.SetMat4f("model", glm::mat4(1.0f));
        container_shader.SetMat4f("view", cam.GetViewMatrix());
        container_shader.SetMat4f("projection", glm::perspective(cam.zoom_, s_aspect_ratio, 0.1f, 100.0f));
        container_shader.Unbind();

        glm::vec3 ambient_light(0.02f);
        glm::vec3 diffuse_light(0.5f);
        glm::vec3 specular_light(1.0f);

        // Configure Directional Light
        glm::vec3 dir_light_direction(2.0f, -1.0f, -3.0f);
        container_shader.Use();
        container_shader.SetFloat("material.shininess", 32.0f);
        container_shader.SetVec3f("dir_light.direction", dir_light_direction);
        container_shader.SetVec3f("dir_light.ambient", ambient_light);
        container_shader.SetVec3f("dir_light.diffuse", diffuse_light);
        container_shader.SetVec3f("dir_light.specular", specular_light);
        container_shader.Unbind();

        // Configure lighting maps
        Texture container_diffuse_map("../images/container2.png", GL_RGBA);
        Texture container_specular_map("../images/container2_specular.png", GL_RGBA);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        container_diffuse_map.Activate(0, container_shader, "material.diffuse_map");
        container_specular_map.Activate(1, container_shader, "material.specular_map");

        object_3d light_obj(container_obj.vbo, 36);
        Shader light_shader("../src/demo/shaders/ch17/light.vs", "../src/demo/shaders/ch17/light.fs");
        glm::vec3 light_pos = -10.0f * dir_light_direction;
        glm::mat4 light_model(1.0f);
        light_model = glm::translate(light_model, light_pos);
        light_model = glm::scale(light_model, glm::vec3(0.2f, 0.2f, 0.2f));

        light_shader.Use();
        light_shader.SetMat4f("model", light_model);
        light_shader.SetMat4f("view", cam.GetViewMatrix());
        light_shader.SetMat4f("projection", glm::perspective(cam.zoom_, s_aspect_ratio, 0.1f, 100.0f));
        light_shader.Unbind();

        return_type output{};
        output.containers.push_back(container_obj);
        output.container_shaders.push_back(container_shader);
        output.light = light_obj;
        output.light_shader = light_shader;

        return output;
    }

    void Run(return_type& inputs, f32 DeltaTime)
    {
        s_delta_time = DeltaTime;

        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view(cam.GetViewMatrix());
        glm::mat4 projection(glm::perspective(cam.zoom_, s_aspect_ratio, 0.1f, 100.0f));
        for (int i = 0; i < inputs.containers.size(); i++)
        {
            inputs.container_shaders[i].Use();
            inputs.container_shaders[i].SetMat4f("view", view);
            inputs.container_shaders[i].SetMat4f("projection", projection);
            inputs.container_shaders[i].SetVec3f("view_pos", cam.position_);
            inputs.container_shaders[i].Unbind();
            inputs.containers[i].Draw(inputs.container_shaders[i]);
        }

        inputs.light_shader.Use();
        inputs.light_shader.SetMat4f("view", view);
        inputs.light_shader.SetMat4f("projection", projection);
        inputs.light_shader.Unbind();
        inputs.light.Draw(inputs.light_shader);
    }

} // namespace ch17

#endif // CH17