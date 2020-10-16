#include "pch.h"
#include "game.h"
#include "scene.h"
#include "input_manager.h"
#include "debug_tools.h"
#include "graphics/framebuffer.h"
#include "graphics/quad.h"
#include "graphics/opengl/opengl_framebuffer.h"
#include "graphics/opengl/opengl_quad.h"
#include "../resource.h"
#include "ui/menu_manager.h"
#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
namespace thebanana {
	game* g_game = NULL;
	game::game(const std::string& title) {
		this->m_frame = 0;
		this->m_window = CreateWindowA(BANANA_WINDOW_CLASS_NAME, title.c_str(), WS_VISIBLE | WS_SYSMENU | WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, HINST_THISCOMPONENT, this);
		RECT r;
		GetWindowRect(this->m_window, &r);
		this->m_aspect_ratio = static_cast<float>(r.right - r.left) / static_cast<float>(r.bottom - r.top);
		this->m_viewport = new opengl_viewport(opengl_viewport::viewport_attribs{ this->m_window, 0, 0, 800, 600, 3, 3, opengl_viewport::viewport_attribs::passed_window });
		this->m_scene = new scene(this);
		this->m_input_manager = new input_manager(this);
		if (this->m_input_manager->add_device(0) == input_manager::device_type::keyboard) {
			std::vector<input_manager::dinput_device> enumerated_devices = this->m_input_manager->get_enumerated_devices();
			for (size_t i = 0; i < enumerated_devices.size(); i++) {
				if (input_manager::get_device_type(enumerated_devices[i]) == input_manager::device_type::mouse) {
					this->m_input_manager->add_device(i);
					break;
				}
			}
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		graphics::set_default_graphics_api(graphics::graphics_api::opengl);
		this->m_menu_manager = new ui::menu_manager(this);
		graphics::opengl::opengl_quad::init_shader();
		this->m_menu_quad = graphics::quad::create(2.f, 2.f, this->m_menu_manager->get_texture());
#ifdef _DEBUG
		debug::init_imgui(this->m_window);
#endif
	}
	game::~game() {
#ifdef _DEBUG
		debug::clean_up_imgui();
#endif
		delete this->m_menu_quad;
		delete this->m_menu_manager;
		delete this->m_scene;
		delete this->m_model_registry;
		delete this->m_viewport;
		delete this->m_input_manager;
		graphics::opengl::opengl_quad::destroy_shader();
	}
	void game::destroy() {
		DestroyWindow(this->m_window);
	}
	void game::update() {
		this->m_frame++;
		if (debug::cursor) {
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		} else {
			SetCursor(NULL);
		}
		this->m_input_manager->update_devices();
		this->m_scene->update();
	}
	void game::render() {
		this->m_menu_manager->draw();
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->m_scene->render();
		if (debug::menus_shown) this->m_menu_quad->render();
#ifdef _DEBUG
		debug::render_imgui(this);
#endif
		this->m_viewport->swap_buffers();
	}
	unsigned int game::get_current_frame() {
		return this->m_frame;
	}
	input_manager* game::get_input_manager() {
		return this->m_input_manager;
	}
	model_registry* game::get_model_registry() {
		return this->m_model_registry;
	}
	ui::menu_manager* game::get_menu_manager() {
		return this->m_menu_manager;
	}
	long long __stdcall game::wndproc(HWND window, unsigned int msg, unsigned long long w_param, long long l_param) {
#ifdef _DEBUG
		if (ImGui_ImplWin32_WndProcHandler(window, msg, w_param, l_param))
			return true;
#endif
		switch (msg) {
		case WM_CREATE:
			SetCursor(NULL);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
		{
			RECT r;
			GetWindowRect(window, &r);
			if (g_game) {
				long width = r.right - r.left;
				long height = r.bottom - r.top;
				g_game->m_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
				glViewport(0, 0, width, height);
			}
		}
		break;
		case WM_CHAR:
			switch (w_param) {
			case VK_ESCAPE:
				DestroyWindow(window);
				break;
			}
			break;
		default:
			return DefWindowProc(window, msg, w_param, l_param);
			break;
		}
		return 0;
	}
	float game::get_aspect_ratio() {
		return this->m_aspect_ratio;
	}
	scene* game::get_scene() {
		return this->m_scene;
	}
	void game::add_model_desc(const model_registry::model_descriptor& desc) {
		this->descriptors.push_back(desc);
	}
	void game::load_models() {
		this->m_model_registry = new model_registry(this->descriptors);
	}
	HWND game::get_window() {
		return this->m_window;
	}
}