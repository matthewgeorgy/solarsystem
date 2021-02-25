#version 450 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D planet_texture;

void
main()
{
    frag_color = texture(planet_texture, tex_coord);
}
