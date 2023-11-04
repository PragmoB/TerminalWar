#pragma once

#include <windows.h>

#include "graphic.h"

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
	// �޴� �׸��� ����
	int num_contents;
	// ���� ���� ���� ���� �׸��� ���� ��
	int max_contents_len;

public:
	Menu(COORD pos, const char* title, bool cursor_is_visible);
	// �ܼ� â�� ����� �巯��
	void appear();
	// �ܼ� â���� ����� ����
	void disappear();

	int get_focus();
	int get_num_contents();
	bool set_focus(int focus);

	// �޴� �׸� content �߰�
	void insert_menu(const char* content);
	// focus�� ����Ű�� �޴� �׸��� x��° ���ں��� color������ content�� ������Ʈ ��
	// �޴� �׸��� ������ ��ä�Ӱ� �����ϱ� ���� ����
	bool update_menu(int focus, SHORT x, const char* content, COLOR color = GRAY);

	// �ʱ� ���·� ������
	void clear();
};