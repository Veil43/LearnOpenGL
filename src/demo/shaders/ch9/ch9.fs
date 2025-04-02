#version 330 core
out vec4 frag_col;

in vec2 tex_coord;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    frag_col = mix(texture(texture1, tex_coord),
                   texture(texture2, tex_coord), 0.2);
}