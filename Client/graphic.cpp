#include <iostream>
#include <windows.h>

#include "graphic.h"
#include "interface.h"
#include "protocol.h"
#include "blocking_queue.h"
#include "Bullet.h"

using namespace std;

char frame[FRAME_Y][FRAME_X];
BlockingQueue<Bullet> fire_queue(50);

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

		for (uint32_t i = 0; i < FRAME_Y; i++)
			cout << frame[i] << endl;

		Sleep(20);
	}
}

/* 경기장 화면 초기화 */

void clear_frame()
{
	for (uint32_t i = 0; i < FRAME_Y; i++)
	{
		for (uint32_t j = 0; j < field.right + 2; j++)
			frame[i][j] = ' ';

		frame[i][field.right + 2 - 1] = NULL;
	}
}

/* 경기장 그리기 */

void draw_field()
{
	uint32_t field_width = field.right - field.left;

	/* 윗변 그리기 */

	for (uint32_t i = 0; i < field_width * 2 - 4; i++)
		frame[field.top][field.left + i] = (i % 2) ? '\xa1' : '\xa6';

	frame[field.top][field.left] = '\xa6';	frame[field.top][field.left + 1] = '\xa3';
	frame[field.top][field.left + field_width * 2 - 6] = '\xa6';
	frame[field.top][field.left + field_width * 2 - 5] = '\xa4';
	frame[field.top][field.left + field_width * 2 - 4] = NULL;

	/* 왼쪽, 오른쪽 변 그리기 */

	for (uint32_t i = field.top + 1; i < field.bottom; i++)
	{
		frame[i][field.left] = '\xa6';
		frame[i][field.left + 1] = '\xa2';

		frame[i][field.right - 2] = '\xa6';
		frame[i][field.right - 1] = '\xa2';
	}

	/* 아랫변 그리기 */
	
	for (uint32_t i = 0; i < field_width * 2 - 4; i++)
		frame[field.bottom - 1][field.left + i] = (i % 2) ? '\xa1' : '\xa6';
	frame[field.bottom - 1][field.left] = '\xa6';
	frame[field.bottom - 1][field.left + 1] = '\xa6';
	frame[field.bottom - 1][field.left + field_width * 2 - 6] = '\xa6';
	frame[field.bottom - 1][field.left + field_width * 2 - 5] = '\xa5';
	frame[field.bottom - 1][field.left + field_width * 2 - 4] = NULL;
}

/* 유저 접속 이벤트 */

void welcome_user(uint32_t x, uint32_t y, char chracter)
{
	frame[field.top + y][field.left + 1 + x] = chracter;
	return;
}

/* 유저 무빙 이벤트 */

void move_user(uint32_t x, uint32_t y, DIRECTION dir, char chracter)
{
	frame[field.top + y][field.left + 1 + x] = ' ';

	switch (dir)
	{
	case UP :
		frame[field.top - 1 + y][field.left + 1 + x] = chracter;
		break;
	case DOWN :
		frame[field.top + 1 + y][field.left + 1 + x] = chracter;
		break;
	case LEFT :
		frame[field.top + y][field.left + x] = chracter;
		break;
	case RIGHT :
		frame[field.top + y][field.left + 2 + x] = chracter;
		break;
	}
}

void fire(Bullet bullet)
{
	fire_queue.Enqueue(bullet);
	return;
}

void render_bullet()
{
	while (1)
	{
		Bullet bullet = fire_queue.Dequeue();
		bullet.fire(field, frame);
	}
}