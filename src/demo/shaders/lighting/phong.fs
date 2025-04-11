#version 330 core
out vec4 frag_color;

in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos;

uniform vec3 light_color;
uniform vec3 view_pos;
uniform vec3 object_color;

void main()
{
    // NOTE:
    // It is typical to perform lighting calculation in view space so that the view position is always at (0,0,0)
    // For this learning exercise we will be using world space however

    // Step 1. Ambient
    float ambient_intensity = 0.1;
    vec3 ambient_color = ambient_intensity * light_color;

    // Step 2. Diffuse
    vec3 light_dir = normalize(light_pos - frag_pos); // why not do the normalize once (at the end)?
    vec3 norm = normalize(normal);
    float diffuse_intensity = max(dot(norm, light_dir), 0.0);
    vec3 diffuse_color = diffuse_intensity * light_color;

    // Step 3. Specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(-frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm); // The reflection vector can be found using the law of reflection stating
    // The Angle of Reflection = The Angle of Incidence and works out to be R = L - 2(L.N)N with:
    // R = Refelction vector
    // L = Light vector (towards the point)
    // N = Normal vector
    
    float specular_intensity = pow(max(dot(view_dir, reflect_dir), 0.0), 32); // Ah yes 32 the magic number (it is shininess here typically powers of 2)
    vec3 specular_color = specular_strength * specular_intensity * light_color;

    vec3 result = (ambient_color + diffuse_color + specular_color) * object_color;
    frag_color = vec4(result, 1.0);
}