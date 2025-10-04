#pragma once
#include "includes.h"
#include "imgui/imgui.h"
#include "mini/ini.h"

typedef struct _Config
{
	std::string configHash;

	struct Menu
	{
		bool opened = true;
		int openKey = VK_INSERT;
		int openIndex = 14;
	} menu;

	struct CFG {
		
		bool antiscreen = true;

		const char* BoxTypes[2] = { "Full Box","Cornered Box" };
		const char* LineTypes[3] = { "Bottom To Enemy","Top To Enemy","Crosshair To Enemy" };
		enum bone_cfg
		{
			head,
			chest,
			pevis,
			left_arm,
			left_hand,
			right_arm,
			right_hand,
			left_knee,
			left_foot,
			right_knee,
			right_foot
		};

		bool bone_selector[11];

		bool b_MenuShow = true;

		bool b_VisualPlayers = false;
		bool b_EspBoxPlayers = true;
		bool b_EspSkeletonPlayers = true;
		bool b_EspLinePlayers = true;
		bool b_EspDistancePlayers = true;
		bool b_EspHealthPlayers = true;
		bool b_EspArmorPlayers = true;
		bool b_EspShieldPlayers = true;
		bool b_EspNamePlayers = true;
		bool b_EspHeadPlayers = true;

		bool b_EspTeam = true;

		bool b_VisualBOT = false;
		bool b_EspBoxBOT = true;
		bool b_EspSkeletonBOT = true;
		bool b_EspLineBOT = true;
		bool b_EspDistanceBOT = true;
		bool b_EspHealthBOT = true;
		bool b_EspShieldBOT = true;
		bool b_EspRealNameBOT = true;
		bool b_EspNameBOT = true;
		bool b_EspHeadBOT = true;

		bool b_AimbotBOT = false;
		bool b_AimbotPlayers = false;
		bool b_AimbotFOV = true;
		bool b_AimbotSmooth = false;
		bool b_AimbotVisibleCheck = true;
		bool b_AimbotMagicBullets = true;

		ImColor PlayersColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersAimedColor = ImColor(255.f, 1.f, 1.f);

		ImColor ShieldColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersNamesColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersBoxColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersSkeletonColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersHeadColor = ImColor(80.f, 80.f, 80.f);
		ImColor PlayersSnaplinesColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersDistanceColor = ImColor(1.f, 255.f, 1.f);
		ImColor PlayersTeamColor = ImColor(1.f, 255.f, 1.f);

		ImColor BOTNamesColor = ImColor(1.f, 80.f, 1.f);
		ImColor BOTBoxColor = ImColor(1.f, 80.f, 1.f);
		ImColor BOTSkeletonColor = ImColor(1.f, 80.f, 1.f);
		ImColor BOTHeadColor = ImColor(1.f, 80.f, 1.f);
		ImColor BOTSnaplinesColor = ImColor(1.f, 80.f, 1.f);
		ImColor BOTDistanceColor = ImColor(1.f, 80.f, 1.f);
		ImColor BOTTeamColor = ImColor(1.f, 80.f, 1.f);

		ImColor ItemNamesColor = ImColor(1.f, 255.f, 1.f);

		ImColor FovColor = ImColor(1.f, 255.f, 1.f);

		ImColor Quality1 = ImColor(255, 255, 255);
		ImColor Quality2 = ImColor(0, 168, 6);
		ImColor Quality3 = ImColor(32, 41, 201);
		ImColor Quality4 = ImColor(201, 0, 212);
		ImColor Quality5 = ImColor(222, 122, 0);
		ImColor Quality6 = ImColor(200, 0, 0);

		ImColor ARColor = ImColor(200, 200, 200);
		ImColor SMGColor = ImColor(200, 200, 200);
		ImColor SNPColor = ImColor(200, 200, 200);
		ImColor SPColor = ImColor(200, 200, 200);
		ImColor SHGColor = ImColor(200, 200, 200);


		int cpuUsage = 0;
		int PlayerBoxType = 0;
		int PlayerLineType = 0;
		int BOTBoxType = 0;
		int BOTLineType = 0;
		int aimLocation = 0;
		int aimLocationBOT = 0;
		int selectedReload = 0; // 0 fast 1 instant
		int tab_index = 0;
		int AimKey = 2;

		int selected_bone = 0; // Variável para armazenar o índice do osso selecionado

		float Smoothing = 2.0f;
		float SmoothingBOT = 2.0f;
		float AimbotFOV = 100.0f;
		float AimbotMaxDist = 100.0f;
		float max_distanceBOT = 1000.0f;
		float max_distancePlayer = 1000.0f;
		float weapon_max_distance = 500.f;
		float ItensLSize = 1.0f;
		float DistanceLSize = 13.0f;
		float NameLSize = 15.0f;
		float ShieldLSize = 14.0f;

		ImColor PlayersNamesColorInvisible = ImColor(255.f, 1.f, 1.f);
		ImColor PlayersBoxColorInvisible = ImColor(255.f, 1.f, 1.f);
		ImColor PlayersSkeletonColorInvisible = ImColor(255.f, 1.f, 1.f);
		ImColor PlayersHeadColorInvisible = ImColor(255.f, 1.f, 1.f);
		ImColor PlayersSnaplinesColorInvisible = ImColor(255.f, 1.f, 1.f);
		ImColor PlayersDistanceColorInvisible = ImColor(255.f, 1.f, 1.f);
		ImColor PlayersTeamColorInvisible = ImColor(255.f, 1.f, 1.f);

		ImColor BOTNamesColorInvisible = ImColor(80.f, 1.f, 1.f);
		ImColor BOTBoxColorInvisible = ImColor(80.f, 1.f, 1.f);
		ImColor BOTSkeletonColorInvisible = ImColor(80.f, 1.f, 1.f);
		ImColor BOTHeadColorInvisible = ImColor(80.f, 1.f, 1.f);
		ImColor BOTSnaplinesColorInvisible = ImColor(80.f, 1.f, 1.f);
		ImColor BOTDistanceColorInvisible = ImColor(80.f, 1.f, 1.f);
		ImColor BOTTeamColorInvisible = ImColor(80.f, 1.f, 1.f);

	}RageMP;

	struct Debug
	{
		bool drawGNames = true;
	} debug;
} Config;

extern Config CFG;

namespace ConfigManager
{
	static const char* ConvertWideCharToChar(const wchar_t* wideStr) {
		if (wideStr == nullptr) {
			return nullptr;
		}

		// Determina o tamanho necessário do buffer para armazenar a string convertida
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
		if (bufferSize == 0) {
			// Erro durante a conversão
			return nullptr;
		}

		// Aloca buffer para armazenar a string convertida
		char* charStr = new char[bufferSize];

		// Converte a string wide char para multibyte (UTF-8 neste caso)
		WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, charStr, bufferSize, nullptr, nullptr);

		return charStr;  // O chamador é responsável por liberar essa memória
	}

	static std::string GetCurrentDir()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		return std::string(buffer).substr(0, pos);
	}

	static void SaveConfigToFile() {

		std::string configName = "RageMP.txt";

		char windowsDir[MAX_PATH];
		if (GetWindowsDirectoryA(windowsDir, MAX_PATH) != 0)
		{
			char driveLetter[4] = { windowsDir[0], ':', '\\', '\0' };
			std::string configFilePath = std::string(driveLetter) + configName;

			mINI::INIFile file(configFilePath.c_str());
			mINI::INIStructure ini;

			ini["Settings"]["b_MenuShow"] = CFG.RageMP.b_MenuShow ? "1" : "0";
			ini["Settings"]["b_VisualPlayers"] = CFG.RageMP.b_VisualPlayers ? "1" : "0";
			ini["Settings"]["b_VisualBOT"] = CFG.RageMP.b_VisualBOT ? "1" : "0";
			ini["Settings"]["b_EspBoxPlayers"] = CFG.RageMP.b_EspBoxPlayers ? "1" : "0";
			ini["Settings"]["b_EspSkeletonPlayers"] = CFG.RageMP.b_EspSkeletonPlayers ? "1" : "0";
			ini["Settings"]["b_EspLinePlayers"] = CFG.RageMP.b_EspLinePlayers ? "1" : "0";
			ini["Settings"]["b_EspDistancePlayers"] = CFG.RageMP.b_EspDistancePlayers ? "1" : "0";
			ini["Settings"]["b_EspHealthPlayers"] = CFG.RageMP.b_EspHealthPlayers ? "1" : "0";
			ini["Settings"]["b_EspShieldPlayers"] = CFG.RageMP.b_EspShieldPlayers ? "1" : "0";
			ini["Settings"]["b_EspNamePlayers"] = CFG.RageMP.b_EspNamePlayers ? "1" : "0";
			ini["Settings"]["b_EspHeadPlayers"] = CFG.RageMP.b_EspHeadPlayers ? "1" : "0";
			ini["Settings"]["b_EspTeam"] = CFG.RageMP.b_EspTeam ? "1" : "0";
			ini["Settings"]["b_EspBoxBOT"] = CFG.RageMP.b_EspBoxBOT ? "1" : "0";
			ini["Settings"]["b_EspSkeletonBOT"] = CFG.RageMP.b_EspSkeletonBOT ? "1" : "0";
			ini["Settings"]["b_EspLineBOT"] = CFG.RageMP.b_EspLineBOT ? "1" : "0";
			ini["Settings"]["b_EspDistanceBOT"] = CFG.RageMP.b_EspDistanceBOT ? "1" : "0";
			ini["Settings"]["b_EspHealthBOT"] = CFG.RageMP.b_EspHealthBOT ? "1" : "0";
			ini["Settings"]["b_EspShieldBOT"] = CFG.RageMP.b_EspShieldBOT ? "1" : "0";
			ini["Settings"]["b_EspRealNameBOT"] = CFG.RageMP.b_EspRealNameBOT ? "1" : "0";
			ini["Settings"]["b_EspNameBOT"] = CFG.RageMP.b_EspNameBOT ? "1" : "0";
			ini["Settings"]["b_EspHeadBOT"] = CFG.RageMP.b_EspHeadBOT ? "1" : "0";
			ini["Settings"]["b_AimbotBOT"] = CFG.RageMP.b_AimbotBOT ? "1" : "0";
			ini["Settings"]["b_AimbotPlayers"] = CFG.RageMP.b_AimbotPlayers ? "1" : "0";
			ini["Settings"]["b_AimbotFOV"] = CFG.RageMP.b_AimbotFOV ? "1" : "0";
			ini["Settings"]["b_AimbotSmooth"] = CFG.RageMP.b_AimbotSmooth ? "1" : "0";
			ini["Settings"]["b_AimbotVisibleCheck"] = CFG.RageMP.b_AimbotVisibleCheck ? "1" : "0";
			ini["Settings"]["b_AimbotMagicBullets"] = CFG.RageMP.b_AimbotMagicBullets ? "1" : "0";
			for (int i = 0; i < 11; ++i) {
				std::string key = "bone_selector_" + std::to_string(i);
				ini["BoneSelector"][key] = CFG.RageMP.bone_selector[i] ? "1" : "0";
			}

			ini["Settings"]["PlayersColorR"] = std::to_string(CFG.RageMP.PlayersColor.Value.x);
			ini["Settings"]["PlayersColorG"] = std::to_string(CFG.RageMP.PlayersColor.Value.y);
			ini["Settings"]["PlayersColorB"] = std::to_string(CFG.RageMP.PlayersColor.Value.z);
			ini["Settings"]["PlayersColorA"] = std::to_string(CFG.RageMP.PlayersColor.Value.w);

			ini["Settings"]["PlayersAimedColorR"] = std::to_string(CFG.RageMP.PlayersAimedColor.Value.x);
			ini["Settings"]["PlayersAimedColorG"] = std::to_string(CFG.RageMP.PlayersAimedColor.Value.y);
			ini["Settings"]["PlayersAimedColorB"] = std::to_string(CFG.RageMP.PlayersAimedColor.Value.z);
			ini["Settings"]["PlayersAimedColorA"] = std::to_string(CFG.RageMP.PlayersAimedColor.Value.w);

			ini["Settings"]["ShieldColorR"] = std::to_string(CFG.RageMP.ShieldColor.Value.x);
			ini["Settings"]["ShieldColorG"] = std::to_string(CFG.RageMP.ShieldColor.Value.y);
			ini["Settings"]["ShieldColorB"] = std::to_string(CFG.RageMP.ShieldColor.Value.z);
			ini["Settings"]["ShieldColorA"] = std::to_string(CFG.RageMP.ShieldColor.Value.w);

			ini["Settings"]["PlayersNamesColorR"] = std::to_string(CFG.RageMP.PlayersNamesColor.Value.x);
			ini["Settings"]["PlayersNamesColorG"] = std::to_string(CFG.RageMP.PlayersNamesColor.Value.y);
			ini["Settings"]["PlayersNamesColorB"] = std::to_string(CFG.RageMP.PlayersNamesColor.Value.z);
			ini["Settings"]["PlayersNamesColorA"] = std::to_string(CFG.RageMP.PlayersNamesColor.Value.w);

			ini["Settings"]["PlayersBoxColorR"] = std::to_string(CFG.RageMP.PlayersBoxColor.Value.x);
			ini["Settings"]["PlayersBoxColorG"] = std::to_string(CFG.RageMP.PlayersBoxColor.Value.y);
			ini["Settings"]["PlayersBoxColorB"] = std::to_string(CFG.RageMP.PlayersBoxColor.Value.z);
			ini["Settings"]["PlayersBoxColorA"] = std::to_string(CFG.RageMP.PlayersBoxColor.Value.w);

			ini["Settings"]["PlayersSkeletonColorR"] = std::to_string(CFG.RageMP.PlayersSkeletonColor.Value.x);
			ini["Settings"]["PlayersSkeletonColorG"] = std::to_string(CFG.RageMP.PlayersSkeletonColor.Value.y);
			ini["Settings"]["PlayersSkeletonColorB"] = std::to_string(CFG.RageMP.PlayersSkeletonColor.Value.z);
			ini["Settings"]["PlayersSkeletonColorA"] = std::to_string(CFG.RageMP.PlayersSkeletonColor.Value.w);

			ini["Settings"]["PlayersHeadColorR"] = std::to_string(CFG.RageMP.PlayersHeadColor.Value.x);
			ini["Settings"]["PlayersHeadColorG"] = std::to_string(CFG.RageMP.PlayersHeadColor.Value.y);
			ini["Settings"]["PlayersHeadColorB"] = std::to_string(CFG.RageMP.PlayersHeadColor.Value.z);
			ini["Settings"]["PlayersHeadColorA"] = std::to_string(CFG.RageMP.PlayersHeadColor.Value.w);

			ini["Settings"]["PlayersSnaplinesColorR"] = std::to_string(CFG.RageMP.PlayersSnaplinesColor.Value.x);
			ini["Settings"]["PlayersSnaplinesColorG"] = std::to_string(CFG.RageMP.PlayersSnaplinesColor.Value.y);
			ini["Settings"]["PlayersSnaplinesColorB"] = std::to_string(CFG.RageMP.PlayersSnaplinesColor.Value.z);
			ini["Settings"]["PlayersSnaplinesColorA"] = std::to_string(CFG.RageMP.PlayersSnaplinesColor.Value.w);

			ini["Settings"]["PlayersDistanceColorR"] = std::to_string(CFG.RageMP.PlayersDistanceColor.Value.x);
			ini["Settings"]["PlayersDistanceColorG"] = std::to_string(CFG.RageMP.PlayersDistanceColor.Value.y);
			ini["Settings"]["PlayersDistanceColorB"] = std::to_string(CFG.RageMP.PlayersDistanceColor.Value.z);
			ini["Settings"]["PlayersDistanceColorA"] = std::to_string(CFG.RageMP.PlayersDistanceColor.Value.w);

			ini["Settings"]["PlayersTeamColorR"] = std::to_string(CFG.RageMP.PlayersTeamColor.Value.x);
			ini["Settings"]["PlayersTeamColorG"] = std::to_string(CFG.RageMP.PlayersTeamColor.Value.y);
			ini["Settings"]["PlayersTeamColorB"] = std::to_string(CFG.RageMP.PlayersTeamColor.Value.z);
			ini["Settings"]["PlayersTeamColorA"] = std::to_string(CFG.RageMP.PlayersTeamColor.Value.w);

			ini["Settings"]["BOTNamesColorR"] = std::to_string(CFG.RageMP.BOTNamesColor.Value.x);
			ini["Settings"]["BOTNamesColorG"] = std::to_string(CFG.RageMP.BOTNamesColor.Value.y);
			ini["Settings"]["BOTNamesColorB"] = std::to_string(CFG.RageMP.BOTNamesColor.Value.z);
			ini["Settings"]["BOTNamesColorA"] = std::to_string(CFG.RageMP.BOTNamesColor.Value.w);

			ini["Settings"]["BOTBoxColorR"] = std::to_string(CFG.RageMP.BOTBoxColor.Value.x);
			ini["Settings"]["BOTBoxColorG"] = std::to_string(CFG.RageMP.BOTBoxColor.Value.y);
			ini["Settings"]["BOTBoxColorB"] = std::to_string(CFG.RageMP.BOTBoxColor.Value.z);
			ini["Settings"]["BOTBoxColorA"] = std::to_string(CFG.RageMP.BOTBoxColor.Value.w);

			ini["Settings"]["BOTSkeletonColorR"] = std::to_string(CFG.RageMP.BOTSkeletonColor.Value.x);
			ini["Settings"]["BOTSkeletonColorG"] = std::to_string(CFG.RageMP.BOTSkeletonColor.Value.y);
			ini["Settings"]["BOTSkeletonColorB"] = std::to_string(CFG.RageMP.BOTSkeletonColor.Value.z);
			ini["Settings"]["BOTSkeletonColorA"] = std::to_string(CFG.RageMP.BOTSkeletonColor.Value.w);

			ini["Settings"]["BOTHeadColorR"] = std::to_string(CFG.RageMP.BOTHeadColor.Value.x);
			ini["Settings"]["BOTHeadColorG"] = std::to_string(CFG.RageMP.BOTHeadColor.Value.y);
			ini["Settings"]["BOTHeadColorB"] = std::to_string(CFG.RageMP.BOTHeadColor.Value.z);
			ini["Settings"]["BOTHeadColorA"] = std::to_string(CFG.RageMP.BOTHeadColor.Value.w);

			ini["Settings"]["BOTSnaplinesColorR"] = std::to_string(CFG.RageMP.BOTSnaplinesColor.Value.x);
			ini["Settings"]["BOTSnaplinesColorG"] = std::to_string(CFG.RageMP.BOTSnaplinesColor.Value.y);
			ini["Settings"]["BOTSnaplinesColorB"] = std::to_string(CFG.RageMP.BOTSnaplinesColor.Value.z);
			ini["Settings"]["BOTSnaplinesColorA"] = std::to_string(CFG.RageMP.BOTSnaplinesColor.Value.w);

			ini["Settings"]["BOTDistanceColorR"] = std::to_string(CFG.RageMP.BOTDistanceColor.Value.x);
			ini["Settings"]["BOTDistanceColorG"] = std::to_string(CFG.RageMP.BOTDistanceColor.Value.y);
			ini["Settings"]["BOTDistanceColorB"] = std::to_string(CFG.RageMP.BOTDistanceColor.Value.z);
			ini["Settings"]["BOTDistanceColorA"] = std::to_string(CFG.RageMP.BOTDistanceColor.Value.w);

			ini["Settings"]["BOTTeamColorR"] = std::to_string(CFG.RageMP.BOTTeamColor.Value.x);
			ini["Settings"]["BOTTeamColorG"] = std::to_string(CFG.RageMP.BOTTeamColor.Value.y);
			ini["Settings"]["BOTTeamColorB"] = std::to_string(CFG.RageMP.BOTTeamColor.Value.z);
			ini["Settings"]["BOTTeamColorA"] = std::to_string(CFG.RageMP.BOTTeamColor.Value.w);

			ini["Settings"]["PlayersNamesColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersNamesColorInvisible.Value.x);
			ini["Settings"]["PlayersNamesColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersNamesColorInvisible.Value.y);
			ini["Settings"]["PlayersNamesColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersNamesColorInvisible.Value.z);
			ini["Settings"]["PlayersNamesColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersNamesColorInvisible.Value.w);

			ini["Settings"]["PlayersBoxColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersBoxColorInvisible.Value.x);
			ini["Settings"]["PlayersBoxColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersBoxColorInvisible.Value.y);
			ini["Settings"]["PlayersBoxColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersBoxColorInvisible.Value.z);
			ini["Settings"]["PlayersBoxColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersBoxColorInvisible.Value.w);

			ini["Settings"]["PlayersSkeletonColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersSkeletonColorInvisible.Value.x);
			ini["Settings"]["PlayersSkeletonColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersSkeletonColorInvisible.Value.y);
			ini["Settings"]["PlayersSkeletonColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersSkeletonColorInvisible.Value.z);
			ini["Settings"]["PlayersSkeletonColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersSkeletonColorInvisible.Value.w);

			ini["Settings"]["PlayersHeadColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersHeadColorInvisible.Value.x);
			ini["Settings"]["PlayersHeadColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersHeadColorInvisible.Value.y);
			ini["Settings"]["PlayersHeadColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersHeadColorInvisible.Value.z);
			ini["Settings"]["PlayersHeadColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersHeadColorInvisible.Value.w);

			ini["Settings"]["PlayersSnaplinesColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersSnaplinesColorInvisible.Value.x);
			ini["Settings"]["PlayersSnaplinesColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersSnaplinesColorInvisible.Value.y);
			ini["Settings"]["PlayersSnaplinesColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersSnaplinesColorInvisible.Value.z);
			ini["Settings"]["PlayersSnaplinesColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersSnaplinesColorInvisible.Value.w);

			ini["Settings"]["PlayersDistanceColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersDistanceColorInvisible.Value.x);
			ini["Settings"]["PlayersDistanceColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersDistanceColorInvisible.Value.y);
			ini["Settings"]["PlayersDistanceColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersDistanceColorInvisible.Value.z);
			ini["Settings"]["PlayersDistanceColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersDistanceColorInvisible.Value.w);

			ini["Settings"]["PlayersTeamColorInvisibleR"] = std::to_string(CFG.RageMP.PlayersTeamColorInvisible.Value.x);
			ini["Settings"]["PlayersTeamColorInvisibleG"] = std::to_string(CFG.RageMP.PlayersTeamColorInvisible.Value.y);
			ini["Settings"]["PlayersTeamColorInvisibleB"] = std::to_string(CFG.RageMP.PlayersTeamColorInvisible.Value.z);
			ini["Settings"]["PlayersTeamColorInvisibleA"] = std::to_string(CFG.RageMP.PlayersTeamColorInvisible.Value.w);

			ini["Settings"]["BOTNamesColorInvisibleR"] = std::to_string(CFG.RageMP.BOTNamesColorInvisible.Value.x);
			ini["Settings"]["BOTNamesColorInvisibleG"] = std::to_string(CFG.RageMP.BOTNamesColorInvisible.Value.y);
			ini["Settings"]["BOTNamesColorInvisibleB"] = std::to_string(CFG.RageMP.BOTNamesColorInvisible.Value.z);
			ini["Settings"]["BOTNamesColorInvisibleA"] = std::to_string(CFG.RageMP.BOTNamesColorInvisible.Value.w);

			ini["Settings"]["BOTBoxColorInvisibleR"] = std::to_string(CFG.RageMP.BOTBoxColorInvisible.Value.x);
			ini["Settings"]["BOTBoxColorInvisibleG"] = std::to_string(CFG.RageMP.BOTBoxColorInvisible.Value.y);
			ini["Settings"]["BOTBoxColorInvisibleB"] = std::to_string(CFG.RageMP.BOTBoxColorInvisible.Value.z);
			ini["Settings"]["BOTBoxColorInvisibleA"] = std::to_string(CFG.RageMP.BOTBoxColorInvisible.Value.w);

			ini["Settings"]["BOTSkeletonColorInvisibleR"] = std::to_string(CFG.RageMP.BOTSkeletonColorInvisible.Value.x);
			ini["Settings"]["BOTSkeletonColorInvisibleG"] = std::to_string(CFG.RageMP.BOTSkeletonColorInvisible.Value.y);
			ini["Settings"]["BOTSkeletonColorInvisibleB"] = std::to_string(CFG.RageMP.BOTSkeletonColorInvisible.Value.z);
			ini["Settings"]["BOTSkeletonColorInvisibleA"] = std::to_string(CFG.RageMP.BOTSkeletonColorInvisible.Value.w);

			ini["Settings"]["BOTHeadColorInvisibleR"] = std::to_string(CFG.RageMP.BOTHeadColorInvisible.Value.x);
			ini["Settings"]["BOTHeadColorInvisibleG"] = std::to_string(CFG.RageMP.BOTHeadColorInvisible.Value.y);
			ini["Settings"]["BOTHeadColorInvisibleB"] = std::to_string(CFG.RageMP.BOTHeadColorInvisible.Value.z);
			ini["Settings"]["BOTHeadColorInvisibleA"] = std::to_string(CFG.RageMP.BOTHeadColorInvisible.Value.w);

			ini["Settings"]["BOTSnaplinesColorInvisibleR"] = std::to_string(CFG.RageMP.BOTSnaplinesColorInvisible.Value.x);
			ini["Settings"]["BOTSnaplinesColorInvisibleG"] = std::to_string(CFG.RageMP.BOTSnaplinesColorInvisible.Value.y);
			ini["Settings"]["BOTSnaplinesColorInvisibleB"] = std::to_string(CFG.RageMP.BOTSnaplinesColorInvisible.Value.z);
			ini["Settings"]["BOTSnaplinesColorInvisibleA"] = std::to_string(CFG.RageMP.BOTSnaplinesColorInvisible.Value.w);

			ini["Settings"]["BOTDistanceColorInvisibleR"] = std::to_string(CFG.RageMP.BOTDistanceColorInvisible.Value.x);
			ini["Settings"]["BOTDistanceColorInvisibleG"] = std::to_string(CFG.RageMP.BOTDistanceColorInvisible.Value.y);
			ini["Settings"]["BOTDistanceColorInvisibleB"] = std::to_string(CFG.RageMP.BOTDistanceColorInvisible.Value.z);
			ini["Settings"]["BOTDistanceColorInvisibleA"] = std::to_string(CFG.RageMP.BOTDistanceColorInvisible.Value.w);

			ini["Settings"]["BOTTeamColorInvisibleR"] = std::to_string(CFG.RageMP.BOTTeamColorInvisible.Value.x);
			ini["Settings"]["BOTTeamColorInvisibleG"] = std::to_string(CFG.RageMP.BOTTeamColorInvisible.Value.y);
			ini["Settings"]["BOTTeamColorInvisibleB"] = std::to_string(CFG.RageMP.BOTTeamColorInvisible.Value.z);
			ini["Settings"]["BOTTeamColorInvisibleA"] = std::to_string(CFG.RageMP.BOTTeamColorInvisible.Value.w);

			ini["Settings"]["ItemNamesColorR"] = std::to_string(CFG.RageMP.ItemNamesColor.Value.x);
			ini["Settings"]["ItemNamesColorG"] = std::to_string(CFG.RageMP.ItemNamesColor.Value.y);
			ini["Settings"]["ItemNamesColorB"] = std::to_string(CFG.RageMP.ItemNamesColor.Value.z);
			ini["Settings"]["ItemNamesColorA"] = std::to_string(CFG.RageMP.ItemNamesColor.Value.w);

			ini["Settings"]["FovColorR"] = std::to_string(CFG.RageMP.FovColor.Value.x);
			ini["Settings"]["FovColorG"] = std::to_string(CFG.RageMP.FovColor.Value.y);
			ini["Settings"]["FovColorB"] = std::to_string(CFG.RageMP.FovColor.Value.z);
			ini["Settings"]["FovColorA"] = std::to_string(CFG.RageMP.FovColor.Value.w);

			ini["Settings"]["Quality1R"] = std::to_string(CFG.RageMP.Quality1.Value.x);
			ini["Settings"]["Quality1G"] = std::to_string(CFG.RageMP.Quality1.Value.y);
			ini["Settings"]["Quality1B"] = std::to_string(CFG.RageMP.Quality1.Value.z);
			ini["Settings"]["Quality1A"] = std::to_string(CFG.RageMP.Quality1.Value.w);

			ini["Settings"]["Quality2R"] = std::to_string(CFG.RageMP.Quality2.Value.x);
			ini["Settings"]["Quality2G"] = std::to_string(CFG.RageMP.Quality2.Value.y);
			ini["Settings"]["Quality2B"] = std::to_string(CFG.RageMP.Quality2.Value.z);
			ini["Settings"]["Quality2A"] = std::to_string(CFG.RageMP.Quality2.Value.w);

			ini["Settings"]["Quality3R"] = std::to_string(CFG.RageMP.Quality3.Value.x);
			ini["Settings"]["Quality3G"] = std::to_string(CFG.RageMP.Quality3.Value.y);
			ini["Settings"]["Quality3B"] = std::to_string(CFG.RageMP.Quality3.Value.z);
			ini["Settings"]["Quality3A"] = std::to_string(CFG.RageMP.Quality3.Value.w);

			ini["Settings"]["Quality4R"] = std::to_string(CFG.RageMP.Quality4.Value.x);
			ini["Settings"]["Quality4G"] = std::to_string(CFG.RageMP.Quality4.Value.y);
			ini["Settings"]["Quality4B"] = std::to_string(CFG.RageMP.Quality4.Value.z);
			ini["Settings"]["Quality4A"] = std::to_string(CFG.RageMP.Quality4.Value.w);

			ini["Settings"]["Quality5R"] = std::to_string(CFG.RageMP.Quality5.Value.x);
			ini["Settings"]["Quality5G"] = std::to_string(CFG.RageMP.Quality5.Value.y);
			ini["Settings"]["Quality5B"] = std::to_string(CFG.RageMP.Quality5.Value.z);
			ini["Settings"]["Quality5A"] = std::to_string(CFG.RageMP.Quality5.Value.w);

			ini["Settings"]["Quality6R"] = std::to_string(CFG.RageMP.Quality6.Value.x);
			ini["Settings"]["Quality6G"] = std::to_string(CFG.RageMP.Quality6.Value.y);
			ini["Settings"]["Quality6B"] = std::to_string(CFG.RageMP.Quality6.Value.z);
			ini["Settings"]["Quality6A"] = std::to_string(CFG.RageMP.Quality6.Value.w);

			ini["Settings"]["ARColorR"] = std::to_string(CFG.RageMP.ARColor.Value.x);
			ini["Settings"]["ARColorG"] = std::to_string(CFG.RageMP.ARColor.Value.y);
			ini["Settings"]["ARColorB"] = std::to_string(CFG.RageMP.ARColor.Value.z);
			ini["Settings"]["ARColorA"] = std::to_string(CFG.RageMP.ARColor.Value.z);

			ini["Settings"]["SMGColorR"] = std::to_string(CFG.RageMP.SMGColor.Value.x);
			ini["Settings"]["SMGColorG"] = std::to_string(CFG.RageMP.SMGColor.Value.y);
			ini["Settings"]["SMGColorB"] = std::to_string(CFG.RageMP.SMGColor.Value.z);
			ini["Settings"]["SMGColorA"] = std::to_string(CFG.RageMP.SMGColor.Value.w);

			ini["Settings"]["SNPColorR"] = std::to_string(CFG.RageMP.SNPColor.Value.x);
			ini["Settings"]["SNPColorG"] = std::to_string(CFG.RageMP.SNPColor.Value.y);
			ini["Settings"]["SNPColorB"] = std::to_string(CFG.RageMP.SNPColor.Value.z);
			ini["Settings"]["SNPColorA"] = std::to_string(CFG.RageMP.SNPColor.Value.w);

			ini["Settings"]["SPColorR"] = std::to_string(CFG.RageMP.SPColor.Value.x);
			ini["Settings"]["SPColorG"] = std::to_string(CFG.RageMP.SPColor.Value.y);
			ini["Settings"]["SPColorB"] = std::to_string(CFG.RageMP.SPColor.Value.z);
			ini["Settings"]["SPColorA"] = std::to_string(CFG.RageMP.SPColor.Value.w);

			ini["Settings"]["SHGColorR"] = std::to_string(CFG.RageMP.SHGColor.Value.x);
			ini["Settings"]["SHGColorG"] = std::to_string(CFG.RageMP.SHGColor.Value.y);
			ini["Settings"]["SHGColorB"] = std::to_string(CFG.RageMP.SHGColor.Value.z);
			ini["Settings"]["SHGColorA"] = std::to_string(CFG.RageMP.SHGColor.Value.w);

			ini["Settings"]["cpuUsage"] = std::to_string(CFG.RageMP.cpuUsage);
			ini["Settings"]["PlayerBoxType"] = std::to_string(CFG.RageMP.PlayerBoxType);
			ini["Settings"]["PlayerLineType"] = std::to_string(CFG.RageMP.PlayerLineType);
			ini["Settings"]["BOTBoxType"] = std::to_string(CFG.RageMP.BOTBoxType);
			ini["Settings"]["BOTLineType"] = std::to_string(CFG.RageMP.BOTLineType);
			ini["Settings"]["aimLocation"] = std::to_string(CFG.RageMP.aimLocation);
			ini["Settings"]["aimLocationBOT"] = std::to_string(CFG.RageMP.aimLocationBOT);
			ini["Settings"]["selectedReload"] = std::to_string(CFG.RageMP.selectedReload);
			ini["Settings"]["tab_index"] = std::to_string(CFG.RageMP.tab_index);
			ini["Settings"]["AimKey"] = std::to_string(CFG.RageMP.AimKey);
			ini["Settings"]["Smoothing"] = std::to_string(CFG.RageMP.Smoothing);
			ini["Settings"]["SmoothingBOT"] = std::to_string(CFG.RageMP.SmoothingBOT);
			ini["Settings"]["AimbotFOV"] = std::to_string(CFG.RageMP.AimbotFOV);
			ini["Settings"]["max_distanceBot"] = std::to_string(CFG.RageMP.max_distanceBOT);
			ini["Settings"]["max_distancePlayer"] = std::to_string(CFG.RageMP.max_distancePlayer);
			ini["Settings"]["weapon_max_distance"] = std::to_string(CFG.RageMP.weapon_max_distance);
			ini["Settings"]["ItensLSize"] = std::to_string(CFG.RageMP.ItensLSize);
			ini["Settings"]["DistanceLSize"] = std::to_string(CFG.RageMP.DistanceLSize);
			ini["Settings"]["NameLSize"] = std::to_string(CFG.RageMP.NameLSize);
			ini["Settings"]["ShieldLSize"] = std::to_string(CFG.RageMP.ShieldLSize);
			ini["Settings"]["selected_bone"] = std::to_string(CFG.RageMP.selected_bone);
			ini["Settings"]["AimbotMaxDist"] = std::to_string(CFG.RageMP.AimbotMaxDist);
			file.write(ini);
		}
		
	}

	static void LoadConfigFromFile() {

		std::string configName = "RageMP.txt";

		char windowsDir[MAX_PATH];
		if (GetWindowsDirectoryA(windowsDir, MAX_PATH) != 0)
		{
			char driveLetter[4] = { windowsDir[0], ':', '\\', '\0' };
			std::string configFilePath = std::string(driveLetter) + configName;

			mINI::INIFile file(configFilePath.c_str());
			mINI::INIStructure ini;

			file.read(ini);

			std::istringstream(ini["Settings"]["b_MenuShow"]) >> CFG.RageMP.b_MenuShow;
			std::istringstream(ini["Settings"]["b_VisualPlayers"]) >> CFG.RageMP.b_VisualPlayers;
			std::istringstream(ini["Settings"]["b_VisualBOT"]) >> CFG.RageMP.b_VisualBOT;
			std::istringstream(ini["Settings"]["b_EspBoxPlayers"]) >> CFG.RageMP.b_EspBoxPlayers;
			std::istringstream(ini["Settings"]["b_EspSkeletonPlayers"]) >> CFG.RageMP.b_EspSkeletonPlayers;
			std::istringstream(ini["Settings"]["b_EspLinePlayers"]) >> CFG.RageMP.b_EspLinePlayers;
			std::istringstream(ini["Settings"]["b_EspDistancePlayers"]) >> CFG.RageMP.b_EspDistancePlayers;
			std::istringstream(ini["Settings"]["b_EspHealthPlayers"]) >> CFG.RageMP.b_EspHealthPlayers;
			std::istringstream(ini["Settings"]["b_EspShieldPlayers"]) >> CFG.RageMP.b_EspShieldPlayers;
			std::istringstream(ini["Settings"]["b_EspNamePlayers"]) >> CFG.RageMP.b_EspNamePlayers;
			std::istringstream(ini["Settings"]["b_EspHeadPlayers"]) >> CFG.RageMP.b_EspHeadPlayers;
			std::istringstream(ini["Settings"]["b_EspTeam"]) >> CFG.RageMP.b_EspTeam;
			std::istringstream(ini["Settings"]["b_EspBoxBOT"]) >> CFG.RageMP.b_EspBoxBOT;
			std::istringstream(ini["Settings"]["b_EspSkeletonBOT"]) >> CFG.RageMP.b_EspSkeletonBOT;
			std::istringstream(ini["Settings"]["b_EspLineBOT"]) >> CFG.RageMP.b_EspLineBOT;
			std::istringstream(ini["Settings"]["b_EspDistanceBOT"]) >> CFG.RageMP.b_EspDistanceBOT;
			std::istringstream(ini["Settings"]["b_EspHealthBOT"]) >> CFG.RageMP.b_EspHealthBOT;
			std::istringstream(ini["Settings"]["b_EspShieldBOT"]) >> CFG.RageMP.b_EspShieldBOT;
			std::istringstream(ini["Settings"]["b_EspRealNameBOT"]) >> CFG.RageMP.b_EspRealNameBOT;
			std::istringstream(ini["Settings"]["b_EspNameBOT"]) >> CFG.RageMP.b_EspNameBOT;
			std::istringstream(ini["Settings"]["b_EspHeadBOT"]) >> CFG.RageMP.b_EspHeadBOT;
			std::istringstream(ini["Settings"]["b_AimbotBOT"]) >> CFG.RageMP.b_AimbotBOT;
			std::istringstream(ini["Settings"]["b_AimbotPlayers"]) >> CFG.RageMP.b_AimbotPlayers;
			std::istringstream(ini["Settings"]["b_AimbotFOV"]) >> CFG.RageMP.b_AimbotFOV;
			std::istringstream(ini["Settings"]["AimbotMaxDist"]) >> CFG.RageMP.AimbotMaxDist;
			std::istringstream(ini["Settings"]["b_AimbotSmooth"]) >> CFG.RageMP.b_AimbotSmooth;
			std::istringstream(ini["Settings"]["b_AimbotVisibleCheck"]) >> CFG.RageMP.b_AimbotVisibleCheck;
			std::istringstream(ini["Settings"]["b_AimbotMagicBullets"]) >> CFG.RageMP.b_AimbotMagicBullets;

			// Carregar os valores do array bone_selector
			for (int i = 0; i < 11; ++i) {
				std::string key = "bone_selector_" + std::to_string(i);
				int value;
				std::istringstream(ini["BoneSelector"][key]) >> value;
				CFG.RageMP.bone_selector[i] = value;
			}

			std::istringstream(ini["Settings"]["PlayersColorR"]) >> CFG.RageMP.PlayersColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersColorG"]) >> CFG.RageMP.PlayersColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersColorB"]) >> CFG.RageMP.PlayersColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersColorA"]) >> CFG.RageMP.PlayersColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersAimedColorR"]) >> CFG.RageMP.PlayersAimedColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersAimedColorG"]) >> CFG.RageMP.PlayersAimedColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersAimedColorB"]) >> CFG.RageMP.PlayersAimedColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersAimedColorA"]) >> CFG.RageMP.PlayersAimedColor.Value.w;

			std::istringstream(ini["Settings"]["ShieldColorR"]) >> CFG.RageMP.ShieldColor.Value.x;
			std::istringstream(ini["Settings"]["ShieldColorG"]) >> CFG.RageMP.ShieldColor.Value.y;
			std::istringstream(ini["Settings"]["ShieldColorB"]) >> CFG.RageMP.ShieldColor.Value.z;
			std::istringstream(ini["Settings"]["ShieldColorA"]) >> CFG.RageMP.ShieldColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersNamesColorR"]) >> CFG.RageMP.PlayersNamesColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersNamesColorG"]) >> CFG.RageMP.PlayersNamesColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersNamesColorB"]) >> CFG.RageMP.PlayersNamesColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersNamesColorA"]) >> CFG.RageMP.PlayersNamesColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersBoxColorR"]) >> CFG.RageMP.PlayersBoxColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersBoxColorG"]) >> CFG.RageMP.PlayersBoxColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersBoxColorB"]) >> CFG.RageMP.PlayersBoxColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersBoxColorA"]) >> CFG.RageMP.PlayersBoxColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersSkeletonColorR"]) >> CFG.RageMP.PlayersSkeletonColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersSkeletonColorG"]) >> CFG.RageMP.PlayersSkeletonColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersSkeletonColorB"]) >> CFG.RageMP.PlayersSkeletonColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersSkeletonColorA"]) >> CFG.RageMP.PlayersSkeletonColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersHeadColorR"]) >> CFG.RageMP.PlayersHeadColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersHeadColorG"]) >> CFG.RageMP.PlayersHeadColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersHeadColorB"]) >> CFG.RageMP.PlayersHeadColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersHeadColorA"]) >> CFG.RageMP.PlayersHeadColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersSnaplinesColorR"]) >> CFG.RageMP.PlayersSnaplinesColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersSnaplinesColorG"]) >> CFG.RageMP.PlayersSnaplinesColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersSnaplinesColorB"]) >> CFG.RageMP.PlayersSnaplinesColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersSnaplinesColorA"]) >> CFG.RageMP.PlayersSnaplinesColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersDistanceColorR"]) >> CFG.RageMP.PlayersDistanceColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersDistanceColorG"]) >> CFG.RageMP.PlayersDistanceColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersDistanceColorB"]) >> CFG.RageMP.PlayersDistanceColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersDistanceColorA"]) >> CFG.RageMP.PlayersDistanceColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersTeamColorR"]) >> CFG.RageMP.PlayersTeamColor.Value.x;
			std::istringstream(ini["Settings"]["PlayersTeamColorG"]) >> CFG.RageMP.PlayersTeamColor.Value.y;
			std::istringstream(ini["Settings"]["PlayersTeamColorB"]) >> CFG.RageMP.PlayersTeamColor.Value.z;
			std::istringstream(ini["Settings"]["PlayersTeamColorA"]) >> CFG.RageMP.PlayersTeamColor.Value.w;

			std::istringstream(ini["Settings"]["BOTNamesColorR"]) >> CFG.RageMP.BOTNamesColor.Value.x;
			std::istringstream(ini["Settings"]["BOTNamesColorG"]) >> CFG.RageMP.BOTNamesColor.Value.y;
			std::istringstream(ini["Settings"]["BOTNamesColorB"]) >> CFG.RageMP.BOTNamesColor.Value.z;
			std::istringstream(ini["Settings"]["BOTNamesColorA"]) >> CFG.RageMP.BOTNamesColor.Value.w;

			std::istringstream(ini["Settings"]["BOTBoxColorR"]) >> CFG.RageMP.BOTBoxColor.Value.x;
			std::istringstream(ini["Settings"]["BOTBoxColorG"]) >> CFG.RageMP.BOTBoxColor.Value.y;
			std::istringstream(ini["Settings"]["BOTBoxColorB"]) >> CFG.RageMP.BOTBoxColor.Value.z;
			std::istringstream(ini["Settings"]["BOTBoxColorA"]) >> CFG.RageMP.BOTBoxColor.Value.w;

			std::istringstream(ini["Settings"]["BOTSkeletonColorR"]) >> CFG.RageMP.BOTSkeletonColor.Value.x;
			std::istringstream(ini["Settings"]["BOTSkeletonColorG"]) >> CFG.RageMP.BOTSkeletonColor.Value.y;
			std::istringstream(ini["Settings"]["BOTSkeletonColorB"]) >> CFG.RageMP.BOTSkeletonColor.Value.z;
			std::istringstream(ini["Settings"]["BOTSkeletonColorA"]) >> CFG.RageMP.BOTSkeletonColor.Value.w;

			std::istringstream(ini["Settings"]["BOTHeadColorR"]) >> CFG.RageMP.BOTHeadColor.Value.x;
			std::istringstream(ini["Settings"]["BOTHeadColorG"]) >> CFG.RageMP.BOTHeadColor.Value.y;
			std::istringstream(ini["Settings"]["BOTHeadColorB"]) >> CFG.RageMP.BOTHeadColor.Value.z;
			std::istringstream(ini["Settings"]["BOTHeadColorA"]) >> CFG.RageMP.BOTHeadColor.Value.w;

			std::istringstream(ini["Settings"]["BOTSnaplinesColorR"]) >> CFG.RageMP.BOTSnaplinesColor.Value.x;
			std::istringstream(ini["Settings"]["BOTSnaplinesColorG"]) >> CFG.RageMP.BOTSnaplinesColor.Value.y;
			std::istringstream(ini["Settings"]["BOTSnaplinesColorB"]) >> CFG.RageMP.BOTSnaplinesColor.Value.z;
			std::istringstream(ini["Settings"]["BOTSnaplinesColorA"]) >> CFG.RageMP.BOTSnaplinesColor.Value.w;

			std::istringstream(ini["Settings"]["BOTDistanceColorR"]) >> CFG.RageMP.BOTDistanceColor.Value.x;
			std::istringstream(ini["Settings"]["BOTDistanceColorG"]) >> CFG.RageMP.BOTDistanceColor.Value.y;
			std::istringstream(ini["Settings"]["BOTDistanceColorB"]) >> CFG.RageMP.BOTDistanceColor.Value.z;
			std::istringstream(ini["Settings"]["BOTDistanceColorA"]) >> CFG.RageMP.BOTDistanceColor.Value.w;

			std::istringstream(ini["Settings"]["BOTTeamColorR"]) >> CFG.RageMP.BOTTeamColor.Value.x;
			std::istringstream(ini["Settings"]["BOTTeamColorG"]) >> CFG.RageMP.BOTTeamColor.Value.y;
			std::istringstream(ini["Settings"]["BOTTeamColorB"]) >> CFG.RageMP.BOTTeamColor.Value.z;
			std::istringstream(ini["Settings"]["BOTTeamColorA"]) >> CFG.RageMP.BOTTeamColor.Value.w;

			std::istringstream(ini["Settings"]["PlayersNamesColorInvisibleR"]) >> CFG.RageMP.PlayersNamesColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersNamesColorInvisibleG"]) >> CFG.RageMP.PlayersNamesColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersNamesColorInvisibleB"]) >> CFG.RageMP.PlayersNamesColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersNamesColorInvisibleA"]) >> CFG.RageMP.PlayersNamesColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["PlayersBoxColorInvisibleR"]) >> CFG.RageMP.PlayersBoxColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersBoxColorInvisibleG"]) >> CFG.RageMP.PlayersBoxColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersBoxColorInvisibleB"]) >> CFG.RageMP.PlayersBoxColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersBoxColorInvisibleA"]) >> CFG.RageMP.PlayersBoxColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["PlayersSkeletonColorInvisibleR"]) >> CFG.RageMP.PlayersSkeletonColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersSkeletonColorInvisibleG"]) >> CFG.RageMP.PlayersSkeletonColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersSkeletonColorInvisibleB"]) >> CFG.RageMP.PlayersSkeletonColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersSkeletonColorInvisibleA"]) >> CFG.RageMP.PlayersSkeletonColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["PlayersHeadColorInvisibleR"]) >> CFG.RageMP.PlayersHeadColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersHeadColorInvisibleG"]) >> CFG.RageMP.PlayersHeadColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersHeadColorInvisibleB"]) >> CFG.RageMP.PlayersHeadColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersHeadColorInvisibleA"]) >> CFG.RageMP.PlayersHeadColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["PlayersSnaplinesColorInvisibleR"]) >> CFG.RageMP.PlayersSnaplinesColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersSnaplinesColorInvisibleG"]) >> CFG.RageMP.PlayersSnaplinesColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersSnaplinesColorInvisibleB"]) >> CFG.RageMP.PlayersSnaplinesColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersSnaplinesColorInvisibleA"]) >> CFG.RageMP.PlayersSnaplinesColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["PlayersDistanceColorInvisibleR"]) >> CFG.RageMP.PlayersDistanceColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersDistanceColorInvisibleG"]) >> CFG.RageMP.PlayersDistanceColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersDistanceColorInvisibleB"]) >> CFG.RageMP.PlayersDistanceColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersDistanceColorInvisibleA"]) >> CFG.RageMP.PlayersDistanceColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["PlayersTeamColorInvisibleR"]) >> CFG.RageMP.PlayersTeamColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["PlayersTeamColorInvisibleG"]) >> CFG.RageMP.PlayersTeamColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["PlayersTeamColorInvisibleB"]) >> CFG.RageMP.PlayersTeamColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["PlayersTeamColorInvisibleA"]) >> CFG.RageMP.PlayersTeamColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTNamesColorInvisibleR"]) >> CFG.RageMP.BOTNamesColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTNamesColorInvisibleG"]) >> CFG.RageMP.BOTNamesColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTNamesColorInvisibleB"]) >> CFG.RageMP.BOTNamesColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTNamesColorInvisibleA"]) >> CFG.RageMP.BOTNamesColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTBoxColorInvisibleR"]) >> CFG.RageMP.BOTBoxColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTBoxColorInvisibleG"]) >> CFG.RageMP.BOTBoxColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTBoxColorInvisibleB"]) >> CFG.RageMP.BOTBoxColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTBoxColorInvisibleA"]) >> CFG.RageMP.BOTBoxColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTSkeletonColorInvisibleR"]) >> CFG.RageMP.BOTSkeletonColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTSkeletonColorInvisibleG"]) >> CFG.RageMP.BOTSkeletonColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTSkeletonColorInvisibleB"]) >> CFG.RageMP.BOTSkeletonColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTSkeletonColorInvisibleA"]) >> CFG.RageMP.BOTSkeletonColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTHeadColorInvisibleR"]) >> CFG.RageMP.BOTHeadColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTHeadColorInvisibleG"]) >> CFG.RageMP.BOTHeadColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTHeadColorInvisibleB"]) >> CFG.RageMP.BOTHeadColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTHeadColorInvisibleA"]) >> CFG.RageMP.BOTHeadColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTSnaplinesColorInvisibleR"]) >> CFG.RageMP.BOTSnaplinesColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTSnaplinesColorInvisibleG"]) >> CFG.RageMP.BOTSnaplinesColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTSnaplinesColorInvisibleB"]) >> CFG.RageMP.BOTSnaplinesColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTSnaplinesColorInvisibleA"]) >> CFG.RageMP.BOTSnaplinesColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTDistanceColorInvisibleR"]) >> CFG.RageMP.BOTDistanceColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTDistanceColorInvisibleG"]) >> CFG.RageMP.BOTDistanceColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTDistanceColorInvisibleB"]) >> CFG.RageMP.BOTDistanceColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTDistanceColorInvisibleA"]) >> CFG.RageMP.BOTDistanceColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["BOTTeamColorInvisibleR"]) >> CFG.RageMP.BOTTeamColorInvisible.Value.x;
			std::istringstream(ini["Settings"]["BOTTeamColorInvisibleG"]) >> CFG.RageMP.BOTTeamColorInvisible.Value.y;
			std::istringstream(ini["Settings"]["BOTTeamColorInvisibleB"]) >> CFG.RageMP.BOTTeamColorInvisible.Value.z;
			std::istringstream(ini["Settings"]["BOTTeamColorInvisibleA"]) >> CFG.RageMP.BOTTeamColorInvisible.Value.w;

			std::istringstream(ini["Settings"]["ItemNamesColorR"]) >> CFG.RageMP.ItemNamesColor.Value.x;
			std::istringstream(ini["Settings"]["ItemNamesColorG"]) >> CFG.RageMP.ItemNamesColor.Value.y;
			std::istringstream(ini["Settings"]["ItemNamesColorB"]) >> CFG.RageMP.ItemNamesColor.Value.z;
			std::istringstream(ini["Settings"]["ItemNamesColorA"]) >> CFG.RageMP.ItemNamesColor.Value.w;

			std::istringstream(ini["Settings"]["FovColorR"]) >> CFG.RageMP.FovColor.Value.x;
			std::istringstream(ini["Settings"]["FovColorG"]) >> CFG.RageMP.FovColor.Value.y;
			std::istringstream(ini["Settings"]["FovColorB"]) >> CFG.RageMP.FovColor.Value.z;
			std::istringstream(ini["Settings"]["FovColorA"]) >> CFG.RageMP.FovColor.Value.w;

			std::istringstream(ini["Settings"]["Quality1R"]) >> CFG.RageMP.Quality1.Value.x;
			std::istringstream(ini["Settings"]["Quality1G"]) >> CFG.RageMP.Quality1.Value.y;
			std::istringstream(ini["Settings"]["Quality1B"]) >> CFG.RageMP.Quality1.Value.z;
			std::istringstream(ini["Settings"]["Quality1A"]) >> CFG.RageMP.Quality1.Value.w;

			std::istringstream(ini["Settings"]["Quality2R"]) >> CFG.RageMP.Quality2.Value.x;
			std::istringstream(ini["Settings"]["Quality2G"]) >> CFG.RageMP.Quality2.Value.y;
			std::istringstream(ini["Settings"]["Quality2B"]) >> CFG.RageMP.Quality2.Value.z;
			std::istringstream(ini["Settings"]["Quality2A"]) >> CFG.RageMP.Quality2.Value.w;

			std::istringstream(ini["Settings"]["Quality3R"]) >> CFG.RageMP.Quality3.Value.x;
			std::istringstream(ini["Settings"]["Quality3G"]) >> CFG.RageMP.Quality3.Value.y;
			std::istringstream(ini["Settings"]["Quality3B"]) >> CFG.RageMP.Quality3.Value.z;
			std::istringstream(ini["Settings"]["Quality3A"]) >> CFG.RageMP.Quality3.Value.w;

			std::istringstream(ini["Settings"]["Quality4R"]) >> CFG.RageMP.Quality4.Value.x;
			std::istringstream(ini["Settings"]["Quality4G"]) >> CFG.RageMP.Quality4.Value.y;
			std::istringstream(ini["Settings"]["Quality4B"]) >> CFG.RageMP.Quality4.Value.z;
			std::istringstream(ini["Settings"]["Quality4A"]) >> CFG.RageMP.Quality4.Value.w;

			std::istringstream(ini["Settings"]["Quality5R"]) >> CFG.RageMP.Quality5.Value.x;
			std::istringstream(ini["Settings"]["Quality5G"]) >> CFG.RageMP.Quality5.Value.y;
			std::istringstream(ini["Settings"]["Quality5B"]) >> CFG.RageMP.Quality5.Value.z;
			std::istringstream(ini["Settings"]["Quality5A"]) >> CFG.RageMP.Quality5.Value.w;

			std::istringstream(ini["Settings"]["Quality6R"]) >> CFG.RageMP.Quality6.Value.x;
			std::istringstream(ini["Settings"]["Quality6G"]) >> CFG.RageMP.Quality6.Value.y;
			std::istringstream(ini["Settings"]["Quality6B"]) >> CFG.RageMP.Quality6.Value.z;
			std::istringstream(ini["Settings"]["Quality6A"]) >> CFG.RageMP.Quality6.Value.w;

			std::istringstream(ini["Settings"]["ARColorR"]) >> CFG.RageMP.ARColor.Value.x;
			std::istringstream(ini["Settings"]["ARColorG"]) >> CFG.RageMP.ARColor.Value.y;
			std::istringstream(ini["Settings"]["ARColorB"]) >> CFG.RageMP.ARColor.Value.z;
			std::istringstream(ini["Settings"]["ARColorA"]) >> CFG.RageMP.ARColor.Value.w;

			std::istringstream(ini["Settings"]["SMGColorR"]) >> CFG.RageMP.SMGColor.Value.x;
			std::istringstream(ini["Settings"]["SMGColorG"]) >> CFG.RageMP.SMGColor.Value.y;
			std::istringstream(ini["Settings"]["SMGColorB"]) >> CFG.RageMP.SMGColor.Value.z;
			std::istringstream(ini["Settings"]["SMGColorA"]) >> CFG.RageMP.SMGColor.Value.w;

			std::istringstream(ini["Settings"]["SNPColorR"]) >> CFG.RageMP.SNPColor.Value.x;
			std::istringstream(ini["Settings"]["SNPColorG"]) >> CFG.RageMP.SNPColor.Value.y;
			std::istringstream(ini["Settings"]["SNPColorB"]) >> CFG.RageMP.SNPColor.Value.z;
			std::istringstream(ini["Settings"]["SNPColorA"]) >> CFG.RageMP.SNPColor.Value.w;

			std::istringstream(ini["Settings"]["SPColorR"]) >> CFG.RageMP.SPColor.Value.x;
			std::istringstream(ini["Settings"]["SPColorG"]) >> CFG.RageMP.SPColor.Value.y;
			std::istringstream(ini["Settings"]["SPColorB"]) >> CFG.RageMP.SPColor.Value.z;
			std::istringstream(ini["Settings"]["SPColorA"]) >> CFG.RageMP.SPColor.Value.w;

			std::istringstream(ini["Settings"]["SHGColorR"]) >> CFG.RageMP.SHGColor.Value.x;
			std::istringstream(ini["Settings"]["SHGColorG"]) >> CFG.RageMP.SHGColor.Value.y;
			std::istringstream(ini["Settings"]["SHGColorB"]) >> CFG.RageMP.SHGColor.Value.z;
			std::istringstream(ini["Settings"]["SHGColorA"]) >> CFG.RageMP.SHGColor.Value.w;

			std::istringstream(ini["Settings"]["cpuUsage"]) >> CFG.RageMP.cpuUsage;
			std::istringstream(ini["Settings"]["PlayerBoxType"]) >> CFG.RageMP.PlayerBoxType;
			std::istringstream(ini["Settings"]["PlayerLineType"]) >> CFG.RageMP.PlayerLineType;
			std::istringstream(ini["Settings"]["BOTBoxType"]) >> CFG.RageMP.BOTBoxType;
			std::istringstream(ini["Settings"]["BOTLineType"]) >> CFG.RageMP.BOTLineType;
			std::istringstream(ini["Settings"]["aimLocation"]) >> CFG.RageMP.aimLocation;
			std::istringstream(ini["Settings"]["selectedReload"]) >> CFG.RageMP.selectedReload;
			std::istringstream(ini["Settings"]["tab_index"]) >> CFG.RageMP.tab_index;
			std::istringstream(ini["Settings"]["AimKey"]) >> CFG.RageMP.AimKey;
			std::istringstream(ini["Settings"]["Smoothing"]) >> CFG.RageMP.Smoothing;
			std::istringstream(ini["Settings"]["SmoothingBOT"]) >> CFG.RageMP.SmoothingBOT;
			std::istringstream(ini["Settings"]["AimbotFOV"]) >> CFG.RageMP.AimbotFOV;
			std::istringstream(ini["Settings"]["max_distanceBOT"]) >> CFG.RageMP.max_distanceBOT;
			std::istringstream(ini["Settings"]["max_distancePlayer"]) >> CFG.RageMP.max_distancePlayer;
			std::istringstream(ini["Settings"]["weapon_max_distance"]) >> CFG.RageMP.weapon_max_distance;
			std::istringstream(ini["Settings"]["ItensLSize"]) >> CFG.RageMP.ItensLSize;
			std::istringstream(ini["Settings"]["DistanceLSize"]) >> CFG.RageMP.DistanceLSize;
			std::istringstream(ini["Settings"]["NameLSize"]) >> CFG.RageMP.NameLSize;
			std::istringstream(ini["Settings"]["ShieldLSize"]) >> CFG.RageMP.ShieldLSize;
			std::istringstream(ini["Settings"]["selected_bone"]) >> CFG.RageMP.selected_bone;
			std::istringstream(ini["Settings"]["AimbotMaxDist"]) >> CFG.RageMP.AimbotMaxDist;
		}
	}
}