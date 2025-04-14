#ifndef OBJECT_V2_H
#define OBJECT_V2_H

#include "math.h"
#include "types.h"
#include "shader.h"

struct vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

class object_3d
{
private:
    static constexpr u32 kPositionIndex = 0;
    static constexpr u32 kColorIndex = 1;
    static constexpr u32 kNormalIndex = 2;
    static constexpr u32 kUVIndex = 3;

public:
    u32 vao;
    u32 vbo;
    i32 vertex_count;
    i32 index_count;

    object_3d()
        : vao{0}, vbo{0}, m_ebo{0}, vertex_count{0}, index_count{0}, m_init{false}
    {}

    /*
        Contstructs a 3D drawable objects from the given 
    */
    object_3d(vertex* Vertices, i32 VCount, u32 *IndexArray = nullptr, i32 ICount = 0)
        : vertex_count{VCount}, index_count{ICount}, m_init{true}
    {
        if (Vertices == nullptr)
            std::cerr << "ERROR: Cannot build an object_3d with no vertices: [Vertex Array is NULL]\n";
        if (VCount < 1)
            std::cerr << "ERROR: Cannot build an object_3d with no vertices: [Vertex Count < 1]\n";

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VCount * sizeof(vertex), (void*)Vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(kPositionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
        glVertexAttribPointer(kColorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
        glVertexAttribPointer(kNormalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
        glVertexAttribPointer(kUVIndex, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tex_coord));
        glEnableVertexAttribArray(kPositionIndex);
        glEnableVertexAttribArray(kColorIndex);
        glEnableVertexAttribArray(kNormalIndex);
        glEnableVertexAttribArray(kUVIndex);
        
        if (IndexArray != nullptr)
        {
            glGenBuffers(1, &m_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, ICount * sizeof(u32), (void*)IndexArray, GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    /*
        Construct a drawable object from already loaded data.
    */
    object_3d(u32 vbo_in, i32 vcount = 0, u32* index_array = nullptr, i32 icount = 0)
        : vbo{vbo_in}, vertex_count{vcount}, index_count{icount}, m_init{true}
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(kPositionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
        glVertexAttribPointer(kColorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
        glVertexAttribPointer(kNormalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
        glVertexAttribPointer(kUVIndex, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tex_coord));
        glEnableVertexAttribArray(kPositionIndex);
        glEnableVertexAttribArray(kColorIndex);
        glEnableVertexAttribArray(kNormalIndex);
        glEnableVertexAttribArray(kUVIndex);

        if (index_array != nullptr)
        {
            glGenBuffers(1, &m_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(u32), (void*)index_array, GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    void Draw(Shader& s)
    {
        if (!m_init)
        {
            std::cerr << "ERROR: Cannot draw uninitialized object_3d\n";
            return;
        }

        s.Use();

        glBindVertexArray(vao);
        if (index_count == 0)
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        else
            glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

        s.Unbind();
    }

private:
    u32 m_ebo;
    bool m_init;
};

#endif