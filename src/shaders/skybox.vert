#version 450 core

layout (location = 0) in vec3 pos;

out vec3 tex_coords;

uniform mat4 view;
uniform mat4 project;

void
main()
{
    gl_Position = project * view * vec4(pos, 1.0f);
    tex_coords = pos;
}
