#version 330 core
out vec4 frag_col;

in vec3 vertex_color;
in vec2 tex_coord;

uniform sampler2D texture1; // << this is the currently bound texture
uniform sampler2D texture2;
void main()
{
    frag_col = mix(texture(texture1, tex_coord),
                   texture(texture2, vec2(-tex_coord.x, tex_coord.y)), 0.2);
}