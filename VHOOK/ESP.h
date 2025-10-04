#pragma once

extern struct esp_ped_;

class ESP
{
public:
	static void Run();
	static void Skeleton(int i, float esp_matrix[16], ImColor Color);
	static void DrawHealthBar(const ImVec2& top_left, const ImVec2& bottom_right, float health, float maxhealth);
	static void HeadLines(float esp_matrix[16], int entity, ImColor Color);
};

