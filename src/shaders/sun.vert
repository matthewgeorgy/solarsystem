#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 i_tex_coord;

out vec2 tex_coord;

uniform mat4 view;
uniform mat4 project;
uniform mat4 tilt;
uniform mat4 align;
uniform mat4 scale;
uniform mat4 translate;

void
main()
{
    mat4 model = align * translate * scale;
    gl_Position = project * view * tilt * model * vec4(pos, 1.0f);
    tex_coord = i_tex_coord;
}
