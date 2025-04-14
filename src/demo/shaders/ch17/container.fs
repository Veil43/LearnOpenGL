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

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float cutoff_outer;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 view_dir);

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;

uniform DirLight dir_light;
uniform Material material;
uniform vec3 view_pos;

void main()
{
    // 1. Ambient

    // 2. Diffuse

    // 3. Specular

    // 5. Attenuation

    // 6. Final Color

    vec3 view_dir = normalize(view_pos - frag_pos);
    frag_color = vec4(CalcDirLight(dir_light, normal, view_dir), 1.0);
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

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir)
{
    return vec3(0);
}
