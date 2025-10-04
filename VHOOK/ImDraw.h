#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

namespace ImDraw
{
	static std::string string_To_UTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string retStr(pBuf);
		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;
		return retStr;
	}

	static auto DrawCrossHair(const FLOAT aSize, ImU32 aColor) -> VOID
	{
		auto vList = ImGui::GetWindowDrawList();

		auto& io = ImGui::GetIO();

		auto ScreenCenter = ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2);

		vList->AddLine({ ScreenCenter.x ,ScreenCenter.y - (aSize + 1) }, { ScreenCenter.x ,ScreenCenter.y + (aSize + 1) }, aColor, 2);
		vList->AddLine({ ScreenCenter.x - (aSize + 1), ScreenCenter.y }, { ScreenCenter.x + (aSize + 1), ScreenCenter.y }, aColor, 2);
	}

	static auto DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth, float f_thickness, bool b_outline = true) -> VOID
	{
		auto window = ImGui::GetWindowDrawList();
		if (b_outline) {
			window->AddLine(aPoint1, aPoint2, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), f_thickness + 1.f);
		}
		window->AddLine(aPoint1, aPoint2, ImGui::GetColorU32(aColor), f_thickness);
	}

	static auto DrawBox(int x1, int y1, int x2, int y2, ImColor color, float thickness, float rounding, bool outline)
	{
		auto WindowPos = ImGui::GetCurrentWindow()->Pos;

		x1 += WindowPos.x;
		y1 += WindowPos.y;

		x2 += WindowPos.x;
		y2 += WindowPos.y;

		ImGui::GetWindowDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(color), rounding);
	}


	static auto RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> VOID
	{
		auto vList = ImGui::GetWindowDrawList();
		vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
	}

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

	static void HealthBar(float x, float y, float w, float h, int phealth, bool Outlined)
	{
		auto vList = ImGui::GetWindowDrawList();
		if (!vList) return;  // Guard against null pointers.

		int healthValue = max(0, min(phealth, 100));

		ImColor barColor = ImColor(
			min(510 * (100 - healthValue) / 100, 255),
			min(510 * healthValue / 100, 255),
			25,
			255
		);

		if (Outlined) {
			vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);
		}

		RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);
	}

	static void ShieldBar(float x, float y, float w, float h, int pShield, bool Outlined)
	{
		if (pShield > 100) pShield = 100;
		auto vList = ImGui::GetWindowDrawList();
		if (!vList) return;  // Guard against null pointers.

		int shieldValue = max(0, min(pShield, 100));

		ImColor barColor = ImColor(
			25,
			25,
			min(510 * shieldValue / 100, 255),
			255
		);

		if (Outlined) {
			vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);
		}

		RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)pShield), barColor, 0.0f, 0);
	}

	/*static auto DrawOutlinedText(std::string str_text, const ImVec2 v2_pos, float size_t, ImVec4 v4_col) -> VOID
	{
		const char* text = str_text.c_str();
		ImGui::GetWindowDrawList()->AddText(bold, size_t, ImVec2(v2_pos.x - 1, v2_pos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255 * v4_col.w)), text);
		ImGui::GetWindowDrawList()->AddText(bold, size_t, ImVec2(v2_pos.x + 1, v2_pos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255 * v4_col.w)), text);
		ImGui::GetWindowDrawList()->AddText(bold, size_t, ImVec2(v2_pos.x - 1, v2_pos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255 * v4_col.w)), text);
		ImGui::GetWindowDrawList()->AddText(bold, size_t, ImVec2(v2_pos.x + 1, v2_pos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255 * v4_col.w)), text);
		ImGui::GetWindowDrawList()->AddText(bold, size_t, v2_pos, ImGui::GetColorU32(v4_col), text);
	}*/

	static auto DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness) -> VOID
	{
		auto WindowPos = ImGui::GetCurrentWindow()->Pos;

		X += WindowPos.x;
		Y += WindowPos.y;

		auto vList = ImGui::GetWindowDrawList();

		float lineW = (W / 3);
		float lineH = (H / 3);
		//black outlines
		auto col = ImGui::GetColorU32(color);

		//corners
		vList->AddLine(ImVec2(X, Y - thickness / 2), ImVec2(X, Y + lineH), col, thickness);//top left
		vList->AddLine(ImVec2(X - thickness / 2, Y), ImVec2(X + lineW, Y), col, thickness);

		vList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W + thickness / 2, Y), col, thickness);//top right horizontal
		vList->AddLine(ImVec2(X + W, Y - thickness / 2), ImVec2(X + W, Y + lineH), col, thickness);

		vList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H + (thickness / 2)), col, thickness);//bot left
		vList->AddLine(ImVec2(X - thickness / 2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

		vList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W + thickness / 2, Y + H), col, thickness);//bot right
		vList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H + (thickness / 2)), col, thickness);
	}

	static auto DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		auto vList = ImGui::GetWindowDrawList();
		vList->AddCircle(aPoint, aR, aColor, 120, aLineWidth);
	}
	static auto DrawCircle(float x, float y, float radius, ImVec4 color, int segments) -> VOID
	{
		auto vList = ImGui::GetWindowDrawList();
		vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments, .2f);
	}
	static auto DrawString(const ImVec2& aPos, const std::string& aString, ImU32 aColor) -> VOID
	{
		auto vList = ImGui::GetWindowDrawList();
		vList->AddText(aPos, aColor, aString.data());
	}
	static auto TabButton(const char* label, int* index, int val, bool sameline) -> VOID
	{
		if (*index == val)
		{
			if (ImGui::Button(label, ImVec2(80, 25)))
				*index = val;
			if (sameline)
				ImGui::SameLine();
		}
		else
		{
			if (ImGui::Button(label, ImVec2(80, 25)))
				*index = val;
			if (sameline)
				ImGui::SameLine();
		}
	}
}