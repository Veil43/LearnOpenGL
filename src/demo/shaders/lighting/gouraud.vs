#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_position;

uniform vec3 light_color;
// uniform vec3 view_pos; // We are doing things in view space
uniform vec3 object_color;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 vertex_pos = vec3(view * model * vec4(aPos, 1.0));
    vec3 normal = mat3(transpose(inverse(view))) * mat3(transpose(inverse(model))) * aNormal;

    // 1. Ambient
    float ambient_intensity = 0.1;
    vec3 ambient_color = ambient_intensity * light_color;

    // 2. Diffuse
    vec3 light_pos = vec3(view * vec4(light_position, 1.0));
    vec3 light_dir = normalize(light_pos - vertex_pos);
    float diffuse_intensity = max(dot(light_dir, normal), 0.0);
    vec3 diffuse_color = diffuse_intensity * light_color;
    
    // 3. Specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(-vertex_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_intensity = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular_color = specular_strength * specular_intensity * light_color;

    // 4. Final result
    vertex_color  = (ambient_color + diffuse_color + specular_color) * object_color;
}