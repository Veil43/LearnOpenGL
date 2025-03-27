#ifndef CH6
#define CH6

#include "chcommon.h"

namespace ch6
{
    ch_return Start(const char* vsource, const char* fsource)
    {
        vertex vertices[] = 
        {
             0.0f,  0.5,  0.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        u32 indices[]
        {
            0, 1, 2,
        };

        Object3D obj(vertices, 3, indices, 3);
        Shader shader(vsource, fsource);
        return {obj, shader}; 
    }

    void Run(ch_return o)
    {
        o.shader.Use();
        float time_value = glfwGetTime();
        float green_value = sin(time_value) / 2.0f + 0.5f;
        
        int vshader_our_color_location = glGetUniformLocation(o.shader.ID_, "our_color");
        glUniform4f(vshader_our_color_location, 0.0f, green_value, 0.0f, 1.0f);

        o.obj.Draw(o.shader);
    }
} // namespace ch6

#endif // CH6