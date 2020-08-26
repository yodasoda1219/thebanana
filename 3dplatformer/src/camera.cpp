#include "pch.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
camera::camera(player* p) : m_player(p) {
	OutputDebugString(TEXT("created camera\n"));
}
void camera::update() {
	this->prepare_for_update();
	this->m_transform = this->m_player->get_transform() * transform().translate(glm::vec3(0.f, 0.f, -2.f));
}
void camera::render() {
	this->prepare_for_render();
	glm::vec3 offset = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 pos = this->m_transform.get_matrix() * glm::vec4(offset, 1.f);
	glm::vec3 player_pos = this->m_player->get_transform().get_matrix() * glm::vec4(offset, 1.f);
	glm::mat4 rotation = this->m_player->get_transform().get_matrix();
	rotation[3] = glm::vec4(0.f, 0.f, 0.f, rotation[3].w);
	glm::vec3 up = glm::vec3(rotation * glm::vec4(0.f, 1.f, 0.f, 1.f));
	glm::mat4 view = glm::lookAt(pos, player_pos, up);
	this->m_scene->get_shader()->get_uniforms().mat4("view", view);
}