#ifndef CH7
#define CH7
/*
Chapter 7 Textures

[Texture Wrapping Modes]: 
Wrapping modes pecify the behaviour of textures when a coordinate is out of range.
GL_REPEAT: Tiles the textures such that (u,v) -> (frac(u), frac(v)) ie for any a.x where a >= 1 we take x
GL_MIRRORED_REPEAT: Same are GL_REPEAT but mirrors with each repeats
GL_CLAMP_TO_EDGE: As the names implies (u, v) -> (clamp01(u), clamp01(v));
GL_CLAMP_TO_BORDER: Coords outside the range are given a user specified colour.

Setting the wrapping mode:
This is done on each axis (s, t and r) r if for 3D textures
glTextParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTextParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, vec3_color); // GL_CLAMP_TO_BORDER;

[Texture Filtering]
Filtering refers to the way we choose what colour to apply to a uv coordinate. That is, give the fixed number of texels(texture pixel)
on a texture, when ge interpolate between vertices we may get coordinates that sit at the edges of a texel.
We then need to decide whether we should take the texel colour or smooth it. You can think of at as some kind of anti aliasing for textures.
Opengl has two main filtering modes:
GL_LINEAR: which uses (bi)linear interpolation between neighbouring texels (results in a smoothed texture)
GL_NEAREST: snaps to the texel closest you the current fragment's computed uv (results in an aliased or jagged texture)

Setting filtering modes:
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINERA);

[Mipmaps]
When rendering an object that is far away and therefore very small on the screen, it is impractical to sample from a high resolution texture
as this results in artifacting and not to mention wasteful. Thus we resort to using lower resolution textures each half the size of the previous
which we use for smaller and smaller objects. This texture set is what we call a [mipmap].

[Why use mipmaps?]
Consider 4k a texture being applied to a triangle. When the triangle far enough so that only one fragment is on the sreen,
every tiny camera movement changes its NDC coordinates after perspective projection.
Since uvs are computed in screenspace, these tiny shifts result in different uv coordinates at the fragment level. 
So, when sampling a high res (like 4k) texture we might start to see flickering since every time we recompute uvs we end up at
a wildly different texel. The high texel count in our texture makes each jump unpredictable and likely to land on a completely
different colour than the one previously displayed.
Using a lower resolution texture for such small objects means that even when these jump occur, there simply aren't enough texels to 
give us huge large range in smapled colours therefore redusing the flickering.

Using mipmaps:
You can generte mipmaped textures with:
glGenerateMimpmaps
You can set filtereing modes for minification to:
GL_(texture filtering)_MIPMAP_(mipmap filtering)
GL_NEAREST_MIPMAP_LINEAR : interpolates betweens 2 mipmaps closest to to the pixel size and uses NEAREST filtering
GL_LINEAR_MIPMAP_NEAREST : oppiste top
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
*/
#include "chcommon.h"
#include "texture.h"

namespace ch7
{
    ch_return Start(const char* vsource, const char* fsource)
    {
        Vertex vertices[] = 
        {
            // pos             // col           // uv
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        };
        
        u32 indices[] =
        {
            0, 1, 3,
            1, 2, 3,
        };
        
        stbi_set_flip_vertically_on_load(true);

        Texture texture1("../images/container.jpg", GL_RGB);
        Texture texture2("../images/awesomeface.png", GL_RGBA);
        // [Texture Wrapping]
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // [Texture Filtering]
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        Object3D obj(vertices, 4, indices, 6);
        Shader shader(vsource, fsource);
        texture1.Activate(0, shader, "texture1");
        texture2.Activate(1, shader, "texture2");
        return {obj, shader};
    }

    void Run(ch_return o)
    {
        o.obj.Draw(o.shader);
    }
}

#endif // CH7