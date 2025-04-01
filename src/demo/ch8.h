#ifndef CH8
#define CH8

#include "chcommon.h"
#include "texture.h"

namespace ch8
{
    static glm::mat4 trans = glm::mat4(1.0f);
    ch_return Start(Object3D obj, const std::string& vsource, const std::string& fsource)
    {

        Texture texture1("../images/container.jpg", GL_RGB);
        Texture texture2("../images/awesomeface.png", GL_RGBA);
        // [Texture Wrapping]
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // [Texture Filtering]
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Shader shader(vsource.c_str(), fsource.c_str());
        texture1.Activate(texture1.ID_, shader, "texture1");
        texture2.Activate(texture2.ID_, shader, "texture2");

        // For use in ch8::Run
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        return {obj, shader};
    }

    void Run(ch_return o)
    {
        trans = glm::rotate(trans, glm::radians(1.5f), glm::vec3(0.0, 0.0, 1.0));

        o.shader.Use();
        u32 transform_location = glGetUniformLocation(o.shader.ID_, "transform");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));

        o.obj.Draw(o.shader);
    }

} // namespace ch8

#endif