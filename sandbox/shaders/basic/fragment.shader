#version 460 core
uniform sampler2D texture_diffuse1;
out vec4 fragment_color;
in vec2 uv;
uniform bool solid_color;
uniform vec3 fill_color;
struct material_t {
	sampler2D albedo;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};
uniform material_t shader_material;
void main() {
	fragment_color = texture(shader_material.albedo, uv) * vec4(shader_material.diffuse, 1.0);
}