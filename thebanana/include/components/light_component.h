#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	class light_component : public component {
	public:
		struct light_data {
			glm::vec3 color;
			float ambient_strength;
		};
		BANANA_API light_component(gameobject* object);
		BANANA_API light_data get_data();
	};
}