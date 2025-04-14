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
        object_3d container;
        Shader container_shader;
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

    glm::vec3 point_light_positions[]
    {
        glm::vec3( 0.7f, 0.2f,2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3( 0.0f, 0.0f, -3.0f)
    };

    glm::vec3 dir_light_position(0.0f);

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
        Shader container_shader("../src/demo/shaders/ch17/container.vs", "../src/demo/shaders/ch17/container.fsh");
        container_shader.Use();
        container_shader.SetMat4f("model", glm::mat4(1.0f));
        container_shader.SetMat4f("view", cam.GetViewMatrix());
        container_shader.SetMat4f("projection", glm::perspective(cam.zoom_, s_aspect_ratio, 0.1f, 100.0f));
        container_shader.Unbind();

        const glm::vec3 c_ambient_light(0.02f);
        const glm::vec3 c_diffuse_light(0.5f);
        const glm::vec3 c_specular_light(1.0f);

        // Configure Directional Light
        glm::vec3 dir_light_direction(2.0f, -1.0f, 0.6f);
        container_shader.Use();
        container_shader.SetFloat("material.shininess", 32.0f);
        container_shader.SetVec3f("dir_light.direction", dir_light_direction);
        container_shader.SetVec3f("dir_light.ambient", c_ambient_light);
        container_shader.SetVec3f("dir_light.diffuse", c_diffuse_light);
        container_shader.SetVec3f("dir_light.specular", c_specular_light);
        container_shader.Unbind();

        // Configure Point Lights

        const f32 c_linear = 0.09f;
        const f32 c_quadratic = 0.032f;
        container_shader.Use();
        container_shader.SetVec3f("point_lights[0].position", point_light_positions[0]);
        container_shader.SetVec3f("point_lights[1].position", point_light_positions[1]);
        container_shader.SetVec3f("point_lights[2].position", point_light_positions[2]);
        container_shader.SetVec3f("point_lights[3].position", point_light_positions[3]);
        container_shader.SetFloat("point_lights[0].constant", 1.0f);
        container_shader.SetFloat("point_lights[1].constant", 1.0f);
        container_shader.SetFloat("point_lights[2].constant", 1.0f);
        container_shader.SetFloat("point_lights[3].constant", 1.0f);
        container_shader.SetFloat("point_lights[0].linear", c_linear);
        container_shader.SetFloat("point_lights[1].linear", c_linear);
        container_shader.SetFloat("point_lights[2].linear", c_linear);
        container_shader.SetFloat("point_lights[3].linear", c_linear);
        container_shader.SetFloat("point_lights[0].quadratic", c_quadratic);
        container_shader.SetFloat("point_lights[1].quadratic", c_quadratic);
        container_shader.SetFloat("point_lights[2].quadratic", c_quadratic);
        container_shader.SetFloat("point_lights[3].quadratic", c_quadratic);
        container_shader.SetVec3f("point_lights[0].ambient", c_ambient_light);
        container_shader.SetVec3f("point_lights[1].ambient", c_ambient_light);
        container_shader.SetVec3f("point_lights[2].ambient", c_ambient_light);
        container_shader.SetVec3f("point_lights[3].ambient", c_ambient_light);
        container_shader.SetVec3f("point_lights[0].diffuse", c_diffuse_light);
        container_shader.SetVec3f("point_lights[1].diffuse", c_diffuse_light);
        container_shader.SetVec3f("point_lights[2].diffuse", c_diffuse_light);
        container_shader.SetVec3f("point_lights[3].diffuse", c_diffuse_light);
        container_shader.SetVec3f("point_lights[0].specular", c_specular_light);
        container_shader.SetVec3f("point_lights[0].specular", c_specular_light);
        container_shader.SetVec3f("point_lights[0].specular", c_specular_light);
        container_shader.SetVec3f("point_lights[0].specular", c_specular_light);
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
        glm::vec3 light_pos = -30.0f * dir_light_direction;
        dir_light_position = light_pos;
        glm::mat4 light_model(1.0f);
        light_model = glm::translate(light_model, light_pos);
        light_model = glm::scale(light_model, glm::vec3(0.2f, 0.2f, 0.2f));

        light_shader.Use();
        light_shader.SetMat4f("model", light_model);
        light_shader.SetMat4f("view", cam.GetViewMatrix());
        light_shader.SetMat4f("projection", glm::perspective(cam.zoom_, s_aspect_ratio, 0.1f, 100.0f));
        light_shader.Unbind();

        return {container_obj, container_shader, light_obj, light_shader};
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

    void Run(return_type& inputs, f32 DeltaTime)
    {
        s_delta_time = DeltaTime;

        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view(cam.GetViewMatrix());
        glm::mat4 projection(glm::perspective(cam.zoom_, s_aspect_ratio, 0.1f, 100.0f));

        // boxes
        for (int i = 0; i < 10; i++)
        {
            glm::vec3 pos = container_positions[i];
            glm::mat4 model(1.0f);
            model = glm::translate(model, pos);
            f32 angle = 20.0f * i * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            inputs.container_shader.Use();
            inputs.container_shader.SetMat4f("model", model);
            inputs.container_shader.SetMat4f("view", view);
            inputs.container_shader.SetMat4f("projection", projection);
            inputs.container_shader.SetVec3f("view_pos", cam.position_);
            inputs.container_shader.Unbind();
            inputs.container.Draw(inputs.container_shader);
        }

        // directional light
        glm::vec3 pos = dir_light_position;
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(0.2f));

        inputs.light_shader.Use();
        inputs.light_shader.SetMat4f("model", model);
        inputs.light_shader.SetMat4f("view", view);
        inputs.light_shader.SetMat4f("projection", projection);
        inputs.light_shader.Unbind();
        inputs.light.Draw(inputs.light_shader);

        // point lights
        for (int i = 0; i < 4; i++)
        {
            pos = point_light_positions[i];
            model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            inputs.light_shader.Use();
            inputs.light_shader.SetMat4f("model", model);
            inputs.light_shader.SetMat4f("view", view);
            inputs.light_shader.SetMat4f("projection", projection);
            inputs.light_shader.Unbind();
            inputs.light.Draw(inputs.light_shader);
        }
    }

} // namespace ch17

#endif // CH17