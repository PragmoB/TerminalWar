#pragma once

#include <windows.h>
#include <vector>

#include "graphic.h"

template <typename T_ELEMENT>
class Menu
{
private:
	// �ܼ�â �ȿ��� �޴��� ��ġ(���� �� �� �� ����)
	const COORD pos;
	// Ŀ���� y���� ��ġ ��(�� �� = 0, �� �Ʒ� = 1, ...)
	int focus;
	// Ŀ���� ����/���� ����
	bool cursor_is_visible;
	
	// �޴��� ����
	const char* title;
	// ���� ���� ���� ���� �׸��� ���� ��
	int max_contents_len;
	// �׸��� ��
	std::vector<T_ELEMENT> elements;

public:
	Menu(COORD pos, const char* title, bool cursor_is_visible);
	// �ܼ� â�� ����� �巯��
	void appear();
	// �ܼ� â���� ����� ����
	void disappear();

	int get_focus();
	bool set_focus(int focus);
	int get_num_contents();
	// focus�� ����Ű�� ��Ұ� ��ȯ
	T_ELEMENT get_element();
	// element�� focus�� ����
	bool set_focus(T_ELEMENT element);

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