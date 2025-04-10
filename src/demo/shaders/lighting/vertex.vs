#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;    // Same as below
out vec3 frag_pos; // Here each vertex gives its world position. The 3 of them are then interpolated
out vec3 light_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_position;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    frag_pos = vec3(view * model * vec4(aPos, 1.0));
    light_pos = vec3(view * vec4(light_position, 1.0));
    normal = mat3(transpose(inverse(view)))* mat3(transpose(inverse(model))) * aNormal; // we want to rotate the normal WITH the model
    // We if the modle matrix Scales the model, then we need to *un*scale with the inverse.
    // as for the transpose? idk maybe it used to still maintian the rotation. Look this up.
}