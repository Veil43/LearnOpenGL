#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#include "chcommon.h"

namespace ch5
{
    Object3D Start(void);
    void Run(Object3D);

    static u32 shader = 0;
    Object3D Start()
    {
        Vertex vertices[] =
        {
            0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
           -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };

        u32 indices[] =
        {
            0, 1, 3,
            1, 2, 3,
        };

        Object3D obj = Object3D(vertices, 4, indices, 6);

        const std::string kVertexShaderSource = LoadFile("../src/demo/shaders/ch5/ch5.vs");
        const std::string kFragmentShaderSource = LoadFile("../src/demo/shaders/ch5/ch5.fs");
        shader = CreateShaderProgram(kVertexShaderSource.c_str(), kFragmentShaderSource.c_str());

        return obj;
    }

    void Run(Object3D obj)
    {
        obj.Draw(shader);
        // glUseProgram(shader);
        // glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

} // namespace ch5

#endif // HELLO_TRIANGLE_H