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

struct vertex
{
    f32 x, y, z;
};

class Object3D
{
public:
    u32 vao_;
    u32 icount_;

    Object3D()
        : vao_{0}, icount_{0}, vbo_{0}, ebo_{0}
    {}

    Object3D(vertex* vertices, u32 vcount, u32* indices, u32 icount)
    {
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vcount, vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * icount, indices, GL_STATIC_DRAW);
        glVertexAttribPointer(kPositionIndex_, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(kPositionIndex_);
        glBindVertexArray(0);
    }

    Object3D& operator=(const Object3D& other)
    {
        if (this == &other) 
            return *this;
        vao_ = other.vao_;
        icount_ = other.icount_;
        vbo_ = other.vbo_;
        ebo_ = other.ebo_;
        return *this;
    }

    void Draw(u32 shader)
    {
        glBindVertexArray(vao_);
        glUseProgram(shader);
        glDrawElements(GL_TRIANGLES, icount_, GL_UNSIGNED_INT, 0);
    }

private:
    u32 vbo_;
    u32 ebo_;
    const u32 kPositionIndex_ = 0;
};

#endif // OBJECT_H