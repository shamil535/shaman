#include "includes.h"
#include "menu.h"
#include "xorstr.h"
#include "config.h"
#include "process.h"
#include "overlay.h"

extern std::string MenuName;

#define VERSION "1.0"
#define LAST_UPDATE_DATA "11/09/2024"

menu* menu::instance = nullptr;
menu* menu::get_instance()
{
	if (!menu::instance)
		menu::instance = new menu();

	return menu::instance;
}

enum heads {
	HEAD_INFO,
	HEAD_ESP,
	HEAD_AIMBOT,
	HEAD_CONFIG
};

std::unordered_map<int, std::string> keyMap = {
	   {-1, "NONE"},
	   {0x01, "LMouse"}, // Botão esquerdo do mouse
	   {0x02, "RMouse"}, // Botão direito do mouse
	   {0x04, "Middle Mouse"}, // Botão do meio do mouse
	   {0x05, "X1 Mouse"}, // Botão extra 1 do mouse
	   {0x06, "X2 Mouse"}, // Botão extra 2 do mouse
	   {0x08, "Backspace"},
	   {0x09, "Tab"},
	   {0x0D, "Enter"},
	   {0x10, "Shift"},
	   {0x11, "Ctrl"},
	   {0x12, "Alt"},
	   {0x14, "Caps Lock"},
	   {0x1B, "Escape"},
	   {0x20, "Space"},
	   {0x21, "Page Up"},
	   {0x22, "Page Down"},
	   {0x23, "End"},
	   {0x24, "Home"},
	   {0x25, "Left Arrow"},
	   {0x26, "Up Arrow"},
	   {0x27, "Right Arrow"},
	   {0x28, "Down Arrow"},
	   {0x2C, "Print Screen"},
	   {0x2D, "Insert"},
	   {0x2E, "Delete"},
	   {0x30, "0"},
	   {0x31, "1"},
	   {0x32, "2"},
	   {0x33, "3"},
	   {0x34, "4"},
	   {0x35, "5"},
	   {0x36, "6"},
	   {0x37, "7"},
	   {0x38, "8"},
	   {0x39, "9"},
	   {0x41, "A"},
	   {0x42, "B"},
	   {0x43, "C"},
	   {0x44, "D"},
	   {0x45, "E"},
	   {0x46, "F"},
	   {0x47, "G"},
	   {0x48, "H"},
	   {0x49, "I"},
	   {0x4A, "J"},
	   {0x4B, "K"},
	   {0x4C, "L"},
	   {0x4D, "M"},
	   {0x4E, "N"},
	   {0x4F, "O"},
	   {0x50, "P"},
	   {0x51, "Q"},
	   {0x52, "R"},
	   {0x53, "S"},
	   {0x54, "T"},
	   {0x55, "U"},
	   {0x56, "V"},
	   {0x57, "W"},
	   {0x58, "X"},
	   {0x59, "Y"},
	   {0x5A, "Z"},
	   {0x60, "Numpad 0"},
	   {0x61, "Numpad 1"},
	   {0x62, "Numpad 2"},
	   {0x63, "Numpad 3"},
	   {0x64, "Numpad 4"},
	   {0x65, "Numpad 5"},
	   {0x66, "Numpad 6"},
	   {0x67, "Numpad 7"},
	   {0x68, "Numpad 8"},
	   {0x69, "Numpad 9"},
	   {0x6A, "Numpad Multiply"},
	   {0x6B, "Numpad Add"},
	   {0x6C, "Numpad Separator"},
	   {0x6D, "Numpad Subtract"},
	   {0x6E, "Numpad Decimal"},
	   {0x6F, "Numpad Divide"},
	   {0x70, "F1"},
	   {0x71, "F2"},
	   {0x72, "F3"},
	   {0x73, "F4"},
	   {0x74, "F5"},
	   {0x75, "F6"},
	   {0x76, "F7"},
	   {0x77, "F8"},
	   {0x78, "F9"},
	   {0x79, "F10"},
	   {0x7A, "F11"},
	   {0x7B, "F12"},
	   {0x90, "Num Lock"},
	   {0x91, "Scroll Lock"},
	   // Adicione mais mapeamentos conforme necessário
};

std::string keyCodeToString(int keyCode)
{


	auto it = keyMap.find(keyCode);
	if (it != keyMap.end())
	{
		return it->second;
	}
	else {
		return "Unknown Key";
	}
}

bool create_key_combo(int* key, const char* name, int width, char* current_item)
{

	if (!key)
		return false;


	// Combobox
	ImGui::PushItemWidth(width);
	if (ImGui::BeginCombo(name, current_item)) // The second parameter is the label previewed before opening the combo.
	{
		//for (int n = 0; n < keyMap.size(); n++)
		for (auto item : keyMap)
		{
			if (item.second.empty())
				continue;

			bool is_selected = (current_item == item.second.c_str()); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(item.second.c_str(), is_selected))
			{
				// current_items1 = combobox_items1[n];
				*key = item.first;
				strcpy(current_item, item.second.c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

struct tab_element {
	float element_opacity;
};

void menu::inputHandler()
{
	for (int i = 0; i < 5; i++) {
		ImGui::GetIO().MouseDown[i] = false;
	}

	int Button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) {
		Button = 0;
	}

	if (Button != -1) {
		ImGui::GetIO().MouseDown[Button] = true;
	}
}

void menu::draw()
{
#ifdef _DirectX9
	this->inputHandler();
#endif

	ImGui::SetNextWindowSize({ 540, 370 });

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin(xor ("Opp Engine"), nullptr, ImGuiWindowFlags_NoDecoration);
	{

		static int combobox, sliderscalar = 55;
		static auto head_selected = HEAD_ESP;

		auto draw = ImGui::GetWindowDrawList();

		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();

		draw->AddRectFilled({ pos.x , pos.y }, ImVec2(pos.x + size.x, pos.y + 51), ImColor(24, 24, 24), 9.0f, ImDrawFlags_RoundCornersTop);
		draw->AddRectFilledMultiColorRounded({ pos.x, pos.y }, ImVec2(pos.x + 55, pos.y + 51), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), 9.0f, ImDrawFlags_RoundCornersTopLeft);

		ImGui::SetCursorPos({ (float)5, (float)7 });
		ImGui::Image(overlay::get_instance()->ChickenLogoTexture, ImVec2(40, 40));

		if (MenuName == "")
			MenuName = xor ("CHICKEN CHEATS");

		ImVec2 textSize = ImGui::CalcTextSize(MenuName.c_str());
		float adjustedPosX = 180.0f + (textSize.x / 1.4f) - (MenuName.length() > 10 ? textSize.x / 10 : 0); // Ajusta para a direita ou esquerda conforme o tamanho do texto
		draw->AddText(overlay::get_instance()->semibold, 17.0f, ImVec2(pos.x + 49, pos.y + 18), ImColor(192, 203, 229), MenuName.c_str());

		ImGui::SetCursorPos({ adjustedPosX, 19.0f });
		ImGui::BeginGroup();
		{
			/*if (elements::tab("Info", head_selected == HEAD_INFO))
				head_selected = HEAD_INFO;

			ImGui::SameLine();*/

			if (elements::tab(xor ("Esp"), head_selected == HEAD_ESP))
				head_selected = HEAD_ESP;

			ImGui::SameLine();

			if (elements::tab(xor ("Aimbot"), head_selected == HEAD_AIMBOT))
				head_selected = HEAD_AIMBOT;

			ImGui::SameLine();

			if (elements::tab(xor ("Config"), head_selected == HEAD_CONFIG))
				head_selected = HEAD_CONFIG;

			ImGui::EndGroup();
		}


		switch (head_selected)
		{
			/*case HEAD_INFO:
			{
				static char current_cheat_version[256] = VERSION;
				static char current_last_update[11] = LAST_UPDATE_DATA;
				static char current_discord[256];
				static char current_site[256];

				static char* discord = strcpy(current_discord, ("discord.gg/oppressor"));
				static char* site = strcpy(current_site, ("oppressor.xyz"));

				float inicio_y = 85;

				ImGui::SetCursorPos({ 25, inicio_y + (float)(0 * 15) });
				ImGui::Text("Version: %s", current_cheat_version);
				ImGui::SetCursorPos({ 25, inicio_y + (1 * 15) });
				ImGui::Text("Last update: %s", current_last_update);
				ImGui::SetCursorPos({ 25, inicio_y + (2 * 15) });
				ImGui::Text("Discord: %s", current_discord);
				ImGui::SetCursorPos({ 25, inicio_y + (3 * 15) });
				ImGui::Text("Website: %s", site);
				break;
			}*/

		case HEAD_ESP:
		{

			draw->AddText(overlay::get_instance()->medium, 14.0f, ImVec2(pos.x + 70, pos.y + 70), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Esp Selection");
			ImGui::SetCursorPos({ 25, 95 });
			if (ImGui::BeginChild(xor ("##ESPCHILDSELECT"), ImVec2(190, 250), false, ImGuiWindowFlags_NoScrollbar))
			{
				//ImGui::Checkbox("Name", &CFG.RageMP.b_EspNamePlayers);
				ImGui::Checkbox(xor ("Box"), &CFG.RageMP.b_EspBoxPlayers);
				ImGui::Checkbox(xor ("Lines"), &CFG.RageMP.b_EspLinePlayers);
				ImGui::Checkbox(xor ("Distance"), &CFG.RageMP.b_EspDistancePlayers);
				ImGui::Checkbox(xor ("Skeleton"), &CFG.RageMP.b_EspSkeletonPlayers);
				ImGui::Checkbox(xor ("Head Lines"), &CFG.RageMP.b_EspHeadPlayers);
				ImGui::Checkbox(xor ("Health"), &CFG.RageMP.b_EspHealthPlayers);
				//ImGui::Checkbox("Armor", &CFG.RageMP.b_EspArmorPlayers);
				/*ImGui::Checkbox("ID", &cfg::esp::id);
				ImGui::Checkbox("Model", &cfg::esp::model);*/
				ImGui::EndChild();
			}

			draw->AddText(overlay::get_instance()->medium, 14.0f, ImVec2(pos.x + 330, pos.y + 70), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Esp Config");
			ImGui::SetCursorPos({ 285, 95 });
			if (ImGui::BeginChild(xor ("##ESPCHILDCFG"), ImVec2(230, 235), false))
			{
				bool joia;
				static ImColor ColorEsp;
				ImGui::ColorEdit4(xor ("Esp Player"), (float*)&CFG.RageMP.PlayersColor, ImGuiColorEditFlags_NoInputs /*| ImGuiColorEditFlags_NoLabel*/);
				//ImGui::ColorEdit4("Esp Friend", (float*)&cfg::esp::cfg_esp::player_friend, ImGuiColorEditFlags_NoInputs /*| ImGuiColorEditFlags_NoLabel*/);
				//ImGui::ColorEdit4("Esp Player Visible", (float*)&cfg::misc::esp::cfg_esp::player_visible, ImGuiColorEditFlags_NoInputs /*| ImGuiColorEditFlags_NoLabel*/);
				ImGui::ColorEdit4(xor ("Esp Player Aimboted"), (float*)&CFG.RageMP.PlayersAimedColor, ImGuiColorEditFlags_NoInputs /*| ImGuiColorEditFlags_NoLabel*/);
				ImGui::ColorEdit4(xor ("Head Lines"), (float*)&CFG.RageMP.PlayersHeadColor, ImGuiColorEditFlags_NoInputs /*| ImGuiColorEditFlags_NoLabel*/);
				int temp_int = static_cast<int>((CFG.RageMP.ItensLSize - 1.0f) * 50.0f);
				ImGui::SliderInt(xor ("##SizeSLIDERS"), &temp_int, 0, 200, 15, xor ("%.2f Esp Tickness"), ImGuiSliderFlags_NoInput);
				CFG.RageMP.ItensLSize = static_cast<float>(temp_int) / 50.0f + 1.0f;


				ImGui::EndChild();
			}

			break;
		}

		case HEAD_AIMBOT:
		{

			ImGui::SetCursorPos({ 25, 65 });

			ImGui::Checkbox(xor ("Enable Aimbot"), &CFG.RageMP.b_AimbotPlayers);

			{
				static char current_item[256];
				static char* one_time = strcpy(current_item, (char*)keyCodeToString(CFG.RageMP.AimKey).c_str());
				ImGui::SetCursorPos({ (float)size.x - 170, (float)60 });
				create_key_combo(&CFG.RageMP.AimKey, xor ("##AIMBOT_KEY_CHOOSE"), 150, current_item);
			}

			draw->AddLine({ pos.x, pos.y + 90 }, { pos.x + size.x, pos.y + 90 }, ImColor(1.0f, 1.0f, 1.0f, 0.6f));

			ImGui::SetCursorPos({ 25, 115 });
			ImGui::PushItemWidth(200);
			//ImGui::Combo("Local Bone", &CFG.RageMP.type_aimbot, "Normal\00Silent\00\00");
			ImGui::PopItemWidth();

			ImGui::SetCursorPos({ 25, 150 });
			ImGui::Checkbox(xor ("Draw Fov"), &CFG.RageMP.b_AimbotFOV);

			ImGui::SetCursorPos({ 25, 175 });
			ImGui::Checkbox("Magic Bullets", &CFG.RageMP.b_AimbotMagicBullets);

			/*ImGui::SetCursorPos({ 25, 200 });
			ImGui::Checkbox("Check Visibility", &CFG.RageMP.check_visibility);

			ImGui::SetCursorPos({ 25, 225 });
			ImGui::Checkbox("Check Player in Vehicle", &CFG.RageMP.check_players_in_vehicle);*/

			ImGui::PushItemWidth(200);

			// Slider FOV
			int tempFOV = static_cast<int>((CFG.RageMP.AimbotFOV - 1.0f) * (200.0f / (900.0f - 1.0f)));
			ImGui::SetCursorPos({ 25, 250 });
			ImGui::SliderInt(xor ("##FOVSLIDER"), &tempFOV, 0, 200, 15, xor ("%.0f Field Of View"), ImGuiSliderFlags_NoInput);
			CFG.RageMP.AimbotFOV = static_cast<float>(tempFOV) * ((900.0f - 1.0f) / 200.0f) + 1.0f;
			// Slider Max Distance
			int tempMaxDist = static_cast<int>(CFG.RageMP.AimbotMaxDist);
			ImGui::SetCursorPos({ 25, 280 });
			ImGui::SliderInt(xor ("##MAXDISNTACEPLAYERAIM"), &tempMaxDist, 0, 1000, 15, xor ("%.0f Max Player Distance"), ImGuiSliderFlags_NoInput);
			CFG.RageMP.AimbotMaxDist = static_cast<float>(tempMaxDist);
			// Slider Smooth Aim
			int tempSmooth = static_cast<int>((CFG.RageMP.Smoothing - 1.0f) * (200.0f / (30.0f - 1.0f)));
			ImGui::SetCursorPos({ 25, 310 });
			ImGui::SliderInt(xor ("##SMOOTH"), &tempSmooth, 0, 200,15, xor ("%.0f Smooth Aim"), ImGuiSliderFlags_NoInput);
			CFG.RageMP.Smoothing = static_cast<float>(tempSmooth) * ((30.0f - 1.0f) / 200.0f) + 1.0f;


			/*ImGui::SetCursorPos({ 25, 330 });
			ImGui::SliderFloat("##HITPORC", &CFG.RageMP.hitchance, 1, 100, 15, "%d%% Silent Hit Percentage", ImGuiSliderFlags_NoInput);
			*/
			ImGui::PopItemWidth();

			float pos_z = 10;
			ImGui::SetCursorPos({ 320, 90 + pos_z });
			ImGui::Image((void*)overlay::get_instance()->PedBoneTexture, ImVec2(150, 250));

			ImGui::SetCursorPos({ 388, 102 + pos_z });
			ImGui::Checkbox(xor ("##HEADBONE"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::head]);

			ImGui::SetCursorPos({ (float)388, (float)148 + pos_z });
			ImGui::Checkbox(xor ("##PEITO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::chest]);

			ImGui::SetCursorPos({ (float)388, (float)209 + pos_z });
			ImGui::Checkbox(xor ("##PELVIS"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::pevis]);

			ImGui::SetCursorPos({ (float)347, (float)163 + pos_z });
			ImGui::Checkbox(xor ("##BRACOESQUERDO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::left_arm]);

			ImGui::SetCursorPos({ (float)321, (float)209 + pos_z });
			ImGui::Checkbox(xor ("##MAOESQUERDO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::left_hand]);

			ImGui::SetCursorPos({ (float)439, (float)163 + pos_z });
			ImGui::Checkbox(xor ("##BRACODIREITO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::right_arm]);

			ImGui::SetCursorPos({ (float)454, (float)209 + pos_z });
			ImGui::Checkbox(xor ("##MAODIREITO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::right_hand]);

			ImGui::SetCursorPos({ (float)367, (float)265 + pos_z });
			ImGui::Checkbox(xor ("##JOELHOESQUERDO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::left_knee]);

			ImGui::SetCursorPos({ (float)362, (float)321 + pos_z });
			ImGui::Checkbox(xor ("##PEESQUERDO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::left_foot]);

			ImGui::SetCursorPos({ (float)413, (float)265 + pos_z });
			ImGui::Checkbox(xor ("##JOELHODIREITO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::right_knee]);

			ImGui::SetCursorPos({ (float)414, (float)321 + pos_z });
			ImGui::Checkbox(xor ("##PEDIREITO"), &CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::right_foot]);

			bool all_bones_off = true;
			for (int i = 0; i < sizeof(CFG.RageMP.bone_selector); i++)
			{
				if (CFG.RageMP.bone_selector[i])
					all_bones_off = false;
			}
			if (all_bones_off)
				CFG.RageMP.bone_selector[CFG.RageMP.bone_cfg::head] = true;

			break;
		}
		case HEAD_CONFIG:
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.0f, 0.0f, 0.3f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.0f, 0.0f, 0.1f));
			ImGui::SetCursorPos({ 25, 95 });
			ImGui::Checkbox(_("Overlay Protection (Disabling this increases the risk of bans!)"), &CFG.RageMP.antiscreen);
			ImGui::SetCursorPos({ 25, 115 });
			ImGui::Checkbox(_("Prediction visible check"), &CFG.RageMP.b_AimbotVisibleCheck);
			ImGui::SetCursorPos({ 25, 330 });
			if (ImGui::Button(_("Unload Cheat"), { 100,20 })) {
				Sleep(1000);exit(0);
			}
			ImGui::PopStyleColor();
			break;
		}

		}
	}
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
	ImGui::PopStyleVar(1); // Don't forget to Pop()
	ImGui::PopStyleColor(1);


}