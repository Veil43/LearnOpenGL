#version 330 core
out vec4 frag_col;

in vec3 vertex_color;
in vec2 tex_coord;

uniform sampler2D our_texture; // << this is the currently bound texture
void main()
{
    frag_col = texture(our_texture, tex_coord);
}