#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 uv;

out vec2 tex_coord;
out vec3 vertex_color;

uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(pos, 1.0);
    vertex_color = col;
    tex_coord = uv;
}