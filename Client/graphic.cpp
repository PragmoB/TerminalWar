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

/* 프레임 업데이트 */

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

/* 경기장 화면 초기화 */

void clear_frame()
{
	for (uint32_t i = 0; i < 40; i++)
	{
		for (uint32_t j = 0; j < 96; j++)
			frame[i][j] = L' ';
		frame[i][96 - 1] = NULL;
	}
}

/* 경기장 그리기 */

void draw_field()
{
	// 멀티바이트 문제로 인하여 무조건 짝수여야함

	/* 윗변 그리기 */

	for (uint32_t i = 0; i < field_width * 2 - 4; i++)
		frame[field_y][field_x + i] = (i % 2) ? '\xa1' : '\xa6';

	frame[field_y][field_x] = '\xa6';	frame[field_y][field_x + 1] = '\xa3';
	frame[field_y][field_x + field_width * 2 - 6] = '\xa6';
	frame[field_y][field_x + field_width * 2 - 5] = '\xa4';

	/* 왼쪽, 오른쪽 변 그리기 */

	for (uint32_t i = 1; i < field_height; i++)
	{
		frame[field_y + i][field_x] = '\xa6';
		frame[field_y + i][field_x + 1] = '\xa2';

		frame[field_y + i][field_x + field_width - 2] = '\xa6';
		frame[field_y + i][field_x + field_width - 1] = '\xa2';
	}

	/* 아랫변 그리기 */
	
	for (uint32_t i = 0; i < field_width * 2 - 4; i++)
		frame[field_y + field_height - 1][field_x + i] = (i % 2) ? '\xa1' : '\xa6';
	frame[field_y + field_height - 1][field_x] = '\xa6';
	frame[field_y + field_height - 1][field_x + 1] = '\xa6';
	frame[field_y + field_height - 1][field_x + field_width * 2 - 6] = '\xa6';
	frame[field_y + field_height - 1][field_x + field_width * 2 - 5] = '\xa5';
}

/* 유저 접속 이벤트 */

void welcome_user(uint32_t x, uint32_t y, char chracter)
{
	frame[field_y + 1 + y][field_x + 1 + x] = chracter;
	return;
}

/* 유저 무빙 이벤트 */

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