#ifndef TEXTURE_H
#define TEXTURE_H

/*
[Texture units]
A texture unit is a pipeline specific resource to which we can bind a texture. (up to 16 units)
This unit can then be accessed through a uniform sampler2D inside our shader.
We will of course need to set the uniform to the unit that we bound.

We can bind a texture to a texture unit with:
glActivateTexture(GL_TEXTURE0 + unit_number);
glBindTexture(GL_TEXTURE_2D, textureID);
glUserProgram(shaderID)
glUniform1i(glGetUniformLocation(shaderID, "name", unit_number));
*/

class Texture
{
public:
    u32 ID_;
    
    Texture() =delete;
    Texture(Texture&&) =delete;
    Texture(const Texture&) =delete;

    /*
    @param format
    `GL_RGB` for .JPG and `GL_RGBA` for .PNG
    */
    Texture(const char* source, u32 format)
    {
        // Load the image to use as texture
        int width, height, nr_channels;
        u8 *data = stbi_load(source, &width, &height, &nr_channels, 0);
        // Generate a texture
        glGenTextures(1, &ID_);
        glBindTexture(GL_TEXTURE_2D, ID_);    
        if (!data)
        {
            std::cerr << "STB::ERROR:: Failed load texture: " << source << std::endl;;
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }

    void Activate(u32 texture_unit, Shader shader, std::string name)
    {
        shader.Use();
        shader.SetInt(name, texture_unit);
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, ID_);
        glUseProgram(0);
    }
};

#endif // TEXTURE_H