#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in vec2 aTexCoord;

out vec3 normal;
out vec3 frag_pos;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    frag_pos = vec3(model * vec4(aPos, 1.0));
    tex_coord = aTexCoord;
    normal = mat3(transpose(inverse(model))) * aNorm; // move the normal proper still haven't looked into this
}