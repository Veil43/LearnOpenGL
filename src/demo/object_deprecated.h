#ifndef OBJECT_H
#define OBJECT_H

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
DRAWING: We can then draw these arrays using [glDrawArrays] or use [glDrawElements] to draw them according to indices.
*/

#include "types.h"
#include "shader.h"

struct Vertex
{
    struct Position { f32 x, y, z; } position;
    struct Color { f32 r, g, b; } color;
    struct UV { f32 u, v; } uv;
};

class Object3D_deprecated
{
public:
    u32 vao_;
    u32 icount_;
    u32 vcount_;

    Object3D_deprecated()
        : vao_{0}, icount_{0}, vbo_{0}, ebo_{0}, vcount_{0}
    {}

    Object3D_deprecated(Vertex* vertices, u32 vcount, u32* indices, u32 icount)
        : icount_{icount}, vcount_{vcount}
    {
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vcount, vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * icount, indices, GL_STATIC_DRAW);
        glVertexAttribPointer(kPositionIndex_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(kColorIndex_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(kUVIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(kPositionIndex_);
        glEnableVertexAttribArray(kColorIndex_);
        glEnableVertexAttribArray(kUVIndex_);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Object3D_deprecated(f32* vertices, u32 vcount, u32 datasize)
        : icount_{0}, vcount_{vcount}, ebo_{0}
    {
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, datasize, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(kPositionIndex_, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(kPositionIndex_);
        glBindVertexArray(0);
    }

    void Draw(u32 shader)
    {
        glBindVertexArray(vao_);
        glUseProgram(shader);

        if (icount_ > 0)
            glDrawElements(GL_TRIANGLES, icount_, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, vcount_/3);

        glBindVertexArray(0);
        glUseProgram(0);
    }

    void Draw(Shader s)
    {
        glBindVertexArray(vao_);
        s.Use();

        if (icount_ > 0)
            glDrawElements(GL_TRIANGLES, icount_, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, vcount_/3);

        glBindVertexArray(0);
        s.Unbind();
    }

private:
    u32 vbo_;
    u32 ebo_;
    const u32 kPositionIndex_ = 0;
    const u32 kColorIndex_ = 1;
    const u32 kUVIndex_ = 2;
};

#endif // OBJECT_H