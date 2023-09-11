#include <iostream>
#include <windows.h>

#include "graphic.h"
#include "interface.h"
#include "protocol.h"

using namespace std;

char frame[25][170];

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* ������ ������Ʈ */

void print_frame()
{
	while (1)
	{
		gotoxy(0, 0);

		for (uint32_t i = 0; i < 25; i++)
			cout << frame[i] << endl;

		Sleep(20);
	}
}

/* ����� ȭ�� �ʱ�ȭ */

void clear_frame()
{
	for (uint32_t i = 0; i < 40; i++)
	{
		for (uint32_t j = 0; j < 96; j++)
			frame[i][j] = L' ';
		frame[i][96 - 1] = NULL;
	}
}

/* ����� �׸��� */

void draw_field()
{
	// ��Ƽ����Ʈ ������ ���Ͽ� ������ ¦��������

	/* ���� �׸��� */

	for (uint32_t i = 0; i < field_width * 2 - 4; i++)
		frame[field_y][field_x + i] = (i % 2) ? '\xa1' : '\xa6';

	frame[field_y][field_x] = '\xa6';	frame[field_y][field_x + 1] = '\xa3';
	frame[field_y][field_x + field_width * 2 - 6] = '\xa6';
	frame[field_y][field_x + field_width * 2 - 5] = '\xa4';

	/* ����, ������ �� �׸��� */

	for (uint32_t i = 1; i < field_height; i++)
	{
		frame[field_y + i][field_x] = '\xa6';
		frame[field_y + i][field_x + 1] = '\xa2';

		frame[field_y + i][field_x + field_width - 2] = '\xa6';
		frame[field_y + i][field_x + field_width - 1] = '\xa2';
	}

	/* �Ʒ��� �׸��� */
	
	for (uint32_t i = 0; i < field_width * 2 - 4; i++)
		frame[field_y + field_height - 1][field_x + i] = (i % 2) ? '\xa1' : '\xa6';
	frame[field_y + field_height - 1][field_x] = '\xa6';
	frame[field_y + field_height - 1][field_x + 1] = '\xa6';
	frame[field_y + field_height - 1][field_x + field_width * 2 - 6] = '\xa6';
	frame[field_y + field_height - 1][field_x + field_width * 2 - 5] = '\xa5';
}

/* ���� ���� �̺�Ʈ */

void welcome_user(uint32_t x, uint32_t y, char chracter)
{
	frame[field_y + 1 + y][field_x + 1 + x] = chracter;
	return;
}

/* ���� ���� �̺�Ʈ */

void move_user(uint32_t x, uint32_t y, DIRECTION dir, char chracter)
{
	frame[field_y + 1 + y][field_x + 1 + x] = ' ';

	switch (dir)
	{
	case UP :
		frame[field_y + y][field_x + 1 + x] = chracter;
		break;
	case DOWN :
		frame[field_y + 2 + y][field_x + 1 + x] = chracter;
		break;
	case LEFT :
		frame[field_y + 1 + y][field_x + x] = chracter;
		break;
	case RIGHT :
		frame[field_y + 1 + y][field_x + 2 + x] = chracter;
		break;
	}
}