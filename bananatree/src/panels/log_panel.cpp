#include <thebanana.h>
#include "log_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	void log_panel::render() {
		ImGui::Begin("Log", &this->m_open);
		std::string log = thebanana::g_game->get_debug_log();
		ImGui::InputTextMultiline("Log Text", &log, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
		ImGui::BeginChild("Log Text");
		ImGui::SetScrollY(ImGui::GetScrollMaxY());
		ImGui::EndChild();
		ImGui::End();
	}
	std::string log_panel::get_menu_text() {
		return "Log";
	}
}