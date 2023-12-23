#ifndef MENU_HPP
#define MENU_HPP

#include <windows.h>

#include "Graphic.h"
#include "Menu.h"

extern Graphic graphic;

template <typename T_ELEMENT>
Menu<T_ELEMENT>::Menu(COORD pos, const char* title, bool cursor_is_visible) :
	pos(pos), title(title), cursor_is_visible(cursor_is_visible),
	max_contents_len(0), focus(0)
{	}

template <typename T_ELEMENT>
void Menu<T_ELEMENT>::appear()
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
		for (int j = 0; j < max_contents_len; j++)
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
		return elements[get_focus()];
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::set_focus(T_ELEMENT element)
{
	int i;
	for (i = 0; i < elements.size(); i++)
	{
		if (elements[i] == element)
			break;
	}

	return set_focus(i);
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::insert_menu(const char* content, T_ELEMENT element)
{

	COORD pos_temp = pos;
	pos_temp.X += 3;
	pos_temp.Y += (2 + elements.size());

	graphic.draw(pos_temp, content);

	elements.push_back(element);
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::update_menu(int focus, SHORT x, const char* content, COLOR color)
{
	if (focus < 0 || elements.size() <= focus)
		return false;

	COORD pos_temp = pos;
	pos_temp.X += (3 + x);
	pos_temp.Y += (2 + focus);

	graphic.draw(pos_temp, content, color);

	return true;
}

template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::update_menu(T_ELEMENT element, SHORT x, const char* content, COLOR color)
{
	int i = 0;
	for (i = 0; i < elements.size(); i++)
	{
		if (elements[i] == element)
			break;
	}
	return update_menu(i, x, content, color);
}
template <typename T_ELEMENT>
bool Menu<T_ELEMENT>::update_menu(T_ELEMENT element, T_ELEMENT element_after)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i] == element)
		{
			elements[i] = element_after;
			return true;
		}
	}
	return false;
}
template <typename T_ELEMENT>
void Menu<T_ELEMENT>::clear()
{
	max_contents_len = 0;
	focus = 0;
	elements.clear();
}


#endif