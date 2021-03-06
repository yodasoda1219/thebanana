#version 410 core
out vec4 fragment_color;
in vec2 uv;
struct material_t {
	sampler2D tex;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};
uniform material_t shader_material;
void main() {
	fragment_color = texture(shader_material.tex, uv) * vec4(shader_material.diffuse, 1.0);
}