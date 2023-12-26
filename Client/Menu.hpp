#ifndef MENU_HPP
#define MENU_HPP

#include <windows.h>

#include "Graphic.h"
#include "Menu.h"

extern Graphic graphic;

template <typename T_ELEMENT>
const int Menu<T_ELEMENT>::WIDTH = 28;

template <typename T_ELEMENT>
Menu<T_ELEMENT>::Menu(COORD pos, const char* title, bool cursor_is_visible) :
	pos(pos), title(title), cursor_is_visible(cursor_is_visible)
{
	std::thread(&Menu<T_ELEMENT>::render_cooldown, this).detach();
}

template <typename T_ELEMENT>
void Menu<T_ELEMENT>::render_cooldown()
{
	while (1)
	{
		for (int i = 0; i < elements.size(); i++)
		{
			int cooldown = elements[i].next_selectable_time - clock();

			COORD pos_temp;
			pos_temp.X = pos.X + max_contents_len + 4;
			pos_temp.Y = pos.Y + 2 + i;

			if (0 < cooldown)
			{
				char buff[20] = "";
				sprintf_s(buff, "| %2d.%-3d초  ", cooldown / 1000, cooldown % 1000);
				graphic.draw(pos_temp, buff);
			}
			else
				graphic.draw(pos_temp, "           ");
		}
		Sleep(60);
	}
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::set_max_contents_len(int max_contents_len)
{
	int prev_max_contents_len = this->max_contents_len;
	this->max_contents_len = max_contents_len;
	
	// 쿨타임 표시 지우기
	for (int i = 0; i < elements.size(); i++)
	{
		COORD pos_temp;
		pos_temp.X = pos.X + prev_max_contents_len + 4;
		pos_temp.Y = pos.Y + 2 + i;
		graphic.draw(pos_temp, "           ");
	}
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::appear()
{
	COORD pos_temp = pos;
	pos_temp.X += 3;
	graphic.draw(pos_temp, title);

	char str[100] = "";
	for (int i = 0; i < WIDTH * 2; i++)
		str[i] = (i % 2) ? '\xa1' : '\xa6';
	pos_temp = pos;
	pos_temp.Y += 1;
	graphic.draw(pos_temp, str);
	
	if (cursor_is_visible)
	{
		pos_temp.X += 1;
		pos_temp.Y += 1 + focus;
		graphic.draw(pos_temp, '>');
	}
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::disappear()
{
	for (int i = 0; i < elements.size() + 2; i++)
	{
		// 공백 문자열 설정
		char spaces[100] = "";
		for (int j = 0; j < WIDTH; j++)
			spaces[j] = ' ';

		// 공백 출력 위치 설정
		COORD pos_temp = pos;
		pos_temp.Y += i;

		// 출력
		graphic.draw(pos_temp, spaces);
	}
}
template <typename T_ELEMENT>
int Menu<T_ELEMENT>::get_focus()
{
	return focus;
}
template <typename T_ELEMENT>
int Menu<T_ELEMENT>::get_num_contents()
{
	return elements.size();
}
template <typename T_ELEMENT>
int Menu<T_ELEMENT>::get_max_contents_len()
{
	return max_contents_len;
}
template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::set_focus(int focus)
{
	// focus값 유효성 검사
	if (focus < 0 || elements.size() <= focus)
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

template <typename T_ELEMENT>
T_ELEMENT Menu<T_ELEMENT>::get_element()
{
	if (elements.size())
		return elements[get_focus()].value;
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::set_focus(T_ELEMENT element)
{
	for (int i = 0; i < elements.size(); i++)
		if (elements[i].value == element)
			return set_focus(i);
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::set_cooldown(T_ELEMENT element, int cooldown)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i].value == element)
		{
			elements[i].next_selectable_time = clock() + cooldown;
			return true;
		}
	}

	return false;
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::insert_menu(const char* content, T_ELEMENT element)
{
	if (max_contents_len < strlen(content))
		set_max_contents_len(strlen(content));

	COORD pos_temp = pos;
	pos_temp.X += 3;
	pos_temp.Y += (2 + elements.size());
	graphic.draw(pos_temp, content);
	elements.push_back(Element{ element, clock() });
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::update_menu(int focus, SHORT x, const char* content, COLOR color)
{
	if (focus < 0 || elements.size() <= focus)
		return false;

	if (max_contents_len < strlen(content) + x)
		set_max_contents_len(strlen(content) + x);

	COORD pos_temp = pos;
	pos_temp.X += (3 + x);
	pos_temp.Y += (2 + focus);
	graphic.draw(pos_temp, content, color);

	return true;
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::update_menu(T_ELEMENT element, SHORT x, const char* content, COLOR color)
{
	bool result = false;
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i].value == element)
		{
			result = true;
			update_menu(i, x, content, color);
		}
	}

	return result;
}
template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::update_menu(T_ELEMENT element, T_ELEMENT element_after)
{
	bool result = false;
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i].value == element)
		{
			result = true;
			elements[i].value = element_after;
		}
	}
	return result;
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::clear()
{
	set_max_contents_len(0);
	focus = 0;
	elements.clear();
}

#endif