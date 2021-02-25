#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 i_tex_coord;
layout (location = 2) in vec3 normal;

out vec2 tex_coord;
out vec3 n_normal;
out vec3 frag_pos;

uniform mat4 view;
uniform mat4 project;
uniform mat4 tilt;
uniform mat4 align;
uniform mat4 scale;
uniform mat4 translate;
uniform mat4 spin;

void
main()
{
    mat4 model = translate * scale * spin * tilt * align;
    gl_Position = project * view * model * vec4(pos, 1.0f);
    tex_coord = i_tex_coord;
    frag_pos = vec3(model * vec4(pos, 1.0f));
    n_normal = mat3(transpose(inverse(model))) * normal;
}
