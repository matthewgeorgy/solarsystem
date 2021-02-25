#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in mat4 inst_matrix;

out vec3 n_normal;
out vec3 frag_pos;

uniform mat4 view;
uniform mat4 project;
uniform mat4 tilt;
uniform mat4 translate;
uniform mat4 rotate;

void
main()
{
    mat4 model = translate * rotate * tilt * inst_matrix;
    gl_Position = project * view * model * vec4(position, 1.0f);
    frag_pos = vec3(model * vec4(position, 1.0f));
    n_normal = mat3(transpose(inverse(model))) * normal;
}
