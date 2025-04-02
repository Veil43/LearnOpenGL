#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "types.h"
#include "math.h"

std::string LoadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "IO::ERROR:Could not open File: " << path << std::endl;;
        return "";
    }
    std::stringstream buffer;
    buffer <<  file.rdbuf(); // what the fuck is this?
    std::string output = buffer.str();
    file.close();
    return output;
}

namespace ch5
{
    /*
    @param type
    This is a GLenum that can be 
    `GL_VERTEX_SHADER` or `GL_FRAGMENT_SHADER`
    */
    u32
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

    u32
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
}
    
class Shader
{
public:
    u32 ID_;

    Shader() =delete;

    Shader(const char* vertex_path, const char* fragment_path)
    {
        std::string vsource = LoadFile(vertex_path);
        std::string fsource = LoadFile(fragment_path);
        ID_ = ch5::CreateShaderProgram(vsource.c_str(), fsource.c_str());
    }

    void Use()
    {
        glUseProgram(ID_);
    }

    void Unbind()
    {
        glUseProgram(0);
    }

    void SetBool(const std::string& name, bool val) const
    {
        glUniform1i(glGetUniformLocation(ID_, name.c_str()), val);
    }
    void SetInt(const std::string& name, int val) const
    {
        glUniform1i(glGetUniformLocation(ID_, name.c_str()), val);
    }
    void SetFloat(const std::string& name, f32 val) const
    {
        glUniform1f(glGetUniformLocation(ID_, name.c_str()), val);
    }
    void SetMat4f(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};

#endif // SHADER_H