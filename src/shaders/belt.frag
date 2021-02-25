#version 450 core

in vec3 frag_pos;
in vec3 n_normal;

out vec4 frag_color;

vec3 light_pos = vec3(0.0f);
vec3 light_color = vec3(1.0f);
vec3 object_color = vec3(0.55f);

void
main()
{
    // ambient
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * light_color;
    
    // diffuse
    vec3 norm = normalize(n_normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = diff * light_color;

    // result
    vec3 result = (ambient + diffuse) * object_color;
    frag_color = vec4(result, 1.0f);
}
