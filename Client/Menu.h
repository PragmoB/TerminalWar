#pragma once

#define _WINSOCKAPI_
#include <windows.h>
#include <vector>

#include "graphic.h"

template <typename T_ELEMENT>
class Menu
{
private:
	static const int WIDTH;

	// �ܼ�â �ȿ��� �޴��� ��ġ(���� �� �� �� ����)
	const COORD pos;
	// Ŀ���� y���� ��ġ ��(�� �� = 0, �� �Ʒ� = 1, ...)
	int focus = 0;
	// Ŀ���� ����/���� ����
	bool cursor_is_visible;
	
	// �޴��� ����
	const char* title;
	// ���� ���� ���� ���� �׸��� ���� ��
	int max_contents_len = 0;

	typedef struct
	{
		// �׸��� ��
		T_ELEMENT value;
		// �׸��� ���� ���� ���� �ð�
		clock_t next_selectable_time;
	} Element;
	std::vector<Element> elements;
private:
	// ���� ��Ÿ�� ǥ��
	void render_cooldown();
	void set_max_contents_len(int max_contents_len);
public:
	Menu(COORD pos, const char* title, bool cursor_is_visible);
	// �ܼ� â�� ����� �巯��
	void appear();
	// �ܼ� â���� ����� ����
	void disappear();

	int get_focus();
	bool set_focus(int focus);
	int get_num_contents();
	int get_max_contents_len();
	// focus�� ����Ű�� ��Ұ� ��ȯ
	T_ELEMENT get_element();
	// element�� focus�� ����
	bool set_focus(T_ELEMENT element);
	// element�� ���� cooldown(�и�������) ����
	bool set_cooldown(T_ELEMENT element, int cooldown);

	// �޴� �׸� content �߰�
	void insert_menu(const char* content, T_ELEMENT element);
	// focus�� ����Ű�� �޴� �׸��� x��° ���ں��� color������ content�� ������Ʈ ��
	// �޴� �׸��� ������ ��ä�Ӱ� �����ϱ� ���� ����
	bool update_menu(int focus, SHORT x, const char* content, COLOR color = GRAY);
	bool update_menu(T_ELEMENT element, SHORT x, const char* content, COLOR color = GRAY);
	bool update_menu(T_ELEMENT element, T_ELEMENT element_after);

	// �ʱ� ���·� ������
	void clear();
};

#include "Menu.hpp"