#include "pch.h"
#include "graphics/util.h"
#include "opengl/opengl_util.h"
namespace thebanana {
	namespace graphics {
		namespace util {
			graphics_enum get_target(unsigned int texture) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::get_target(texture);
					break;
				default:
					return NONE;
					break;
				}
			}
			bool is_2d(unsigned int texture) {
				return get_target(texture) == TEXTURE_2D;
			}
			unsigned int get_bound_framebuffer() {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::get_bound_framebuffer();
					break;
				default:
					return 0;
					break;
				}
			}
			unsigned int get_current_shader() {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::get_current_shader();
					break;
				default:
					return 0;
					break;
				}
			}
			void unbind_all_textures(const std::vector<graphics_enum>& types) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					opengl::unbind_all_textures(types);
					break;
				default:
					break;
				}
			}
			void get_texture_data(void* texid, void* buffer) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					opengl::get_texture_data(texid, buffer);
					break;
				default:
					break;
				}
			}
			void get_texture_size(void* texid, size_t& width, size_t& height) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					opengl::get_texture_size(texid, width, height);
					break;
				default:
					break;
				}
			}
			graphics_enum to_enum(unsigned int e) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::to_enum(e);
					break;
				default:
					return NONE;
					break;
				}
			}
			unsigned int from_enum(graphics_enum e) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::from_enum(e);
					break;
				default:
					return 0;
					break;
				}
			}
		}
	}
}
