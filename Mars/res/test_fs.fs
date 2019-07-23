#version 460 core

out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D _texture;

void main()
{
    frag_color = texture(_texture, tex_coord);
}