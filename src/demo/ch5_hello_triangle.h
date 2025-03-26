#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#include "types.h"
#include "object.h"

namespace ch5
{
    void Start(void);
    void Run(void);

    /*
    @param type
    This is a GLenum that can be 
    `GL_VERTEX_SHADER` or `GL_FRAGMENT_SHADER`
    */
    static u32 
    CreateShader(u32 type, const char* source)
    {
        u32 shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        int success;
        char info_log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::" 
            << ((type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT")
            << "::COMPILATION_FAILED:\n" << info_log << std::endl;
            return 0;
        }
        return shader;
    }
    
    static u32 
    CreateShaderProgram(const char* vsource , const char* fsource)
    {
        u32 vertex_shader = CreateShader(GL_VERTEX_SHADER, vsource);
        u32 fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fsource);
        u32 shader_program;
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);
        int success;
        char info_log[512];
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader_program, 512, NULL, info_log);
            std::cerr << "ERROR::PROGRAM::FAILED_TO_LINK:\n" << info_log << std::endl;
            return 0;
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return shader_program;
    }

    static u32 shader;
    static u32 vao;
    static Object3D obj;
    void Start()
    {
        vertex vertices[] =
        {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
           -0.5f, -0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f,
        };

        u32 indices[] =
        {
            0, 1, 3,
            1, 2, 3,
        };

        obj = Object3D(vertices, 4, indices, 6);

        const char* kVertexShaderSource = 
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\n";
        const char* kFragmentShaderSource =
        "#version 330 core\n"
        "\n"
        "out vec4 frag_color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";
        shader = CreateShaderProgram(kVertexShaderSource, kFragmentShaderSource);
        std::cout << "Shader: " << shader << std::endl;
    }

    void Run()
    {
        obj.Draw(shader);
        // glUseProgram(shader);
        // glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

} // namespace ch5

#endif // HELLO_TRIANGLE_H