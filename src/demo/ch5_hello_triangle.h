#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#include "types.h"

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
    void Start()
    {
        static f32 vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };

        vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        u32 vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        /*
        [glVertexAttriPointer]
        Defines a pointer in the strictest sense of the term. 
        For example take attribute Pos, we defining a POINTER Pos* and giving OpenGL enough information to do 
        process operations such as Pos*+4 where this translate to something Pos* + 4*Stride, giving us the start address
        of the 4th next Pos.

        [glEnableVertexAttibArray]
        Since we have multiple vertices, each with a Pos attribute, and a known number of vertices,
        Pos* effectively defines an array such that Pos*++ always yields the next Pos attributes in line.
        In fact this apply to any and all attributes for which we defined a glVertexAttribPointer.
        Here we simply enable the attribute array that was assigned to index n where n is a number (0,1,2,3...)

        These pointers and therefore arrays(of attributes) are what a [vao] then stores, along with the buffer holding the data.
        */

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
    }

    void Run()
    {
        glUseProgram(shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

} // namespace ch5

#endif // HELLO_TRIANGLE_H