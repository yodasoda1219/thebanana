#include "pch.h"
#include "game.h"
#include "scene.h"
#include "prop.h"
std::string path_helper(const std::string& original, const std::string& find, const std::string& replace) {
	size_t pos = std::string::npos;
	std::string result = original;
	do {
		if (pos != std::string::npos) {
			result = replace + original.substr(pos + find.length());
		}
		pos = original.find(find, pos + 1);
	} while (pos != std::string::npos);
	return result;
}
std::string waluigi_paths(const std::string& path, void*) {
	return path_helper(path, "Waluigi\\Waluigi\\", "textures\\placeholder\\waluigi\\");
}
std::string kunai_paths(const std::string& path, void*) {
	return path_helper(path, "Ibukis Kunai\\SFV_IBUKI_Kunai\\", "textures\\placeholder\\kunai\\");
}
std::string mk8rr_paths(const std::string& path, void*) {
	return path_helper(path, "Wii U - Mario Kart 8 - Rainbow Road\\", "textures\\placeholder\\mk8rr\\");
}
std::string results_stage_paths(const std::string& path, void*) {
	return path_helper(path, "Results Stage\\Results Stage\\images\\", "textures\\placeholder\\resultsstage\\");
}
void init_game() {
	g_game = new game(TEXT("window"));
	g_game->get_scene()->add_object(new prop("resultsstage"));
	g_game->add_model_desc({ "waluigi", "models/placeholder/waluigi.fbx", waluigi_paths, transform().scale(glm::vec3(0.0005f)) });
	g_game->add_model_desc({ "kunai", "models/placeholder/kunai.obj", kunai_paths, transform() });
	g_game->add_model_desc({ "mk8rr", "models/placeholder/mk8rainbowroad.obj", mk8rr_paths, transform() });
	g_game->add_model_desc({ "resultsstage", "models/placeholder/resultsstage.obj", results_stage_paths, transform().rotate(90.f, glm::vec3(1.f, 0.f, 0.f)) });
	g_game->load_models();
}
void gameloop() {
	g_game->update();
	g_game->render();
}
void clean_up_game() {
	delete g_game;
}
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev, char* cmd_line, int cmd_show) {
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.lpfnWndProc = game::wndproc;
	wc.lpszClassName = TEXT("3dplat");
	wc.hCursor = LoadCursor(HINST_THISCOMPONENT, IDC_ARROW);
	wc.hInstance = HINST_THISCOMPONENT;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	assert(RegisterClass(&wc));
	init_game();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		gameloop();
	}
	clean_up_game();
	return static_cast<int>(msg.wParam);
}