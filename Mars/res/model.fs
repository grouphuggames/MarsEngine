#version 460 core

out vec4 _color;

struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 _position;
in vec3 _normal;
in vec2 _tex_coords;

uniform vec3 view_position;
uniform Material material;
uniform Light light;

void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, _tex_coords).rgb;

	vec3 norm = normalize(_normal);
	vec3 light_direction = normalize(light.position - _position);
	float diff = max(dot(norm, light_direction), 0.f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, _tex_coords).rgb;

	vec3 view_direction = normalize(view_position - _position);
	vec3 reflection_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflection_direction), 0.f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	_color = vec4(result, 1.f);
}