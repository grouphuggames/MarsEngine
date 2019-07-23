#version 460 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

out vec3 _position;
out vec3 _normal;
out vec2 _tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	_position = vec3(model * vec4(in_position, 1.f));
	_normal = mat3(transpose(inverse(model))) * in_normal;
	_tex_coords = in_tex_coords;

	gl_Position = projection * view * vec4(_position, 1.f);
}