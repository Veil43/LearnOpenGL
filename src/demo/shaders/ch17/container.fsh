#version 330 core
out vec4 frag_color;

struct Material
{
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;

uniform DirLight dir_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 view_pos;

void main()
{
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);
    // 1. Directional light
    vec3 result = CalcDirLight(dir_light, norm, view_dir);
    // 2. Point Lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        vec3 other = CalcPointLight(point_lights[i], norm, frag_pos, view_dir);
        result += other;
    }

    frag_color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // Specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 
                     material.shininess);

    // Final result
    vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_map, tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular_map, tex_coord));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    // Diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // Specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0),
                     material.shininess);
    // Attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Final result
    vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_map, tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular_map, tex_coord));

    return attenuation * (ambient + diffuse + specular);
}
