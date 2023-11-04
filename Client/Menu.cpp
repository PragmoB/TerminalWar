#include <windows.h>

#include "Graphic.h"
#include "Menu.h"

extern Graphic graphic;

Menu::Menu(COORD pos, const char* title, bool cursor_is_visible) :
	pos(pos), title(title), cursor_is_visible(cursor_is_visible),
	num_contents(0), max_contents_len(0), focus(0)
{	}

void Menu::appear()
{
	COORD pos_temp = pos;
	pos_temp.X += 3;
	graphic.draw(pos_temp, title);

	char str[49] = "";
	for (int i = 0; i < 48; i++)
		str[i] = (i % 2) ? '\xa1' : '\xa6';
	pos_temp = pos;
	pos_temp.Y += 1;
	graphic.draw(pos_temp, str);
	
	if (cursor_is_visible)
	{
		pos_temp.X += 1;
		pos_temp.Y += 1;
		graphic.draw(pos_temp, '>');
	}
}
void Menu::disappear()
{
	for (int i = 0; i < num_contents + 2; i++)
	{
		// 공백 문자열 설정
		char spaces[100] = "";
		for (int j = 0; j < max_contents_len; j++)
			spaces[j] = ' ';

		// 공백 출력 위치 설정
		COORD pos_temp = pos;
		pos_temp.Y += i;

		// 출력
		graphic.draw(pos_temp, spaces);
	}
}
int Menu::get_focus()
{
	return focus;
}
int Menu::get_num_contents()
{
	return num_contents;
}
bool Menu::set_focus(int focus)
{
	// focus값 유효성 검사
	if (focus < 0 || num_contents <= focus)
		return false;

	if (cursor_is_visible)
	{
		COORD pos_temp = pos;
		pos_temp.X += 1;
		pos_temp.Y += (2 + this->focus);
		graphic.draw(pos_temp, ' ');

		pos_temp = pos;
		pos_temp.X += 1;
		pos_temp.Y += (2 + focus);
		graphic.draw(pos_temp, '>');

		Beep(1000, 100);
	}

	this->focus = focus;

	return true;
}
void Menu::insert_menu(const char* content)
{
	COORD pos_temp = pos;
	pos_temp.X += 3;
	pos_temp.Y += (2 + num_contents);

	graphic.draw(pos_temp, content);

	num_contents++;
}

bool Menu::update_menu(int focus, SHORT x, const char* content, COLOR color)
{
	if (focus < 0 || num_contents <= focus)
		return false;

	COORD pos_temp = pos;
	pos_temp.X += (3 + x);
	pos_temp.Y += (2 + focus);

	graphic.draw(pos_temp, content, color);

	return true;
}

void Menu::clear()
{
	num_contents = 0;
	max_contents_len = 0;
	focus = 0;
}