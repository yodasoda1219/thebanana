#pragma once
#include "graphics/texture.h"
#include "banana_api.h"
namespace thebanana {
	class material {
	public:
		BANANA_API material();
		BANANA_API void set_albedo(const std::string& image_path);
		BANANA_API void set_albedo(void* data, int width, int height, int channels, bool clear_path = true);
		BANANA_API void set_albedo(graphics::texture* texture);
		BANANA_API const std::shared_ptr<graphics::texture>& get_albedo();
		BANANA_API std::string get_albedo_path();
		BANANA_API void set_diffuse(glm::vec3 color);
		BANANA_API void set_specular(glm::vec3 color);
		BANANA_API void set_ambient(glm::vec3 color);
		BANANA_API glm::vec3 get_diffuse();
		BANANA_API glm::vec3 get_specular();
		BANANA_API glm::vec3 get_ambient();
		BANANA_API void set_shininess(float shininess);
		BANANA_API float get_shininess();
		BANANA_API void send_to_shader(unsigned int shader_id, const std::string& uniform_name);
		BANANA_API void set_uuid(unsigned long long uuid);
		BANANA_API unsigned long long get_uuid();
		BANANA_API std::string get_friendly_name();
		BANANA_API void set_friendly_name(const std::string& name);
	private:
		std::shared_ptr<graphics::texture> m_albedo;
		glm::vec3 m_diffuse, m_specular, m_ambient;
		float m_shininess;
		unsigned long long m_uuid;
		std::string m_albedo_path;
		std::string m_friendly_name;
	};
	class material_registry {
	public:
		BANANA_API unsigned long long new_material();
		BANANA_API material* find(unsigned long long uuid);
		BANANA_API void clear();
		BANANA_API size_t get_count();
		BANANA_API material* get(size_t index);
	private:
		std::vector<std::unique_ptr<material>> m_materials;
	};
}