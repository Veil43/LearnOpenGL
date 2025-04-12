#version 330 core
out vec4 frag_color;
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;

uniform Material material;
uniform Light light;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
    // 1. Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));

    // 2. Diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coord));

    // 3. Specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0),
                    material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coord));

    // 4. Emission;
    vec3 emission = texture(material.emission, tex_coord).rgb;

    // 5. Final Result
    vec3 result = ambient + diffuse + specular + emission;
    frag_color = vec4(result, 1.0);
}