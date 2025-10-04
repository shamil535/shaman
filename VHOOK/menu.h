#pragma once

class ImFont;
class menu
{
private:
	static menu* instance;

public:
	static menu* get_instance();
	void inputHandler();

	void draw();

	ImFont* bold;
	ImFont* tab_icons;
	ImFont* logo;
	ImFont* tab_title;
	ImFont* tab_title_icon;
	ImFont* combo_arrow;
};