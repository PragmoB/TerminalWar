#include <iostream>
#include <windows.h>

#include "blocking_queue.h"

#include "Graphic.h"
#include "interface.h"
#include "protocol.h"
#include "Bullet.h"

using namespace std;

// 게임 그래픽 엔진
Graphic graphic;

Graphic::Graphic() : fire_queue(50)
{
	console_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SMALL_RECT rect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(console_buffer, TRUE, &rect);

	COORD win_coords = { field.Left + 2 * field_width + 30, field.Bottom + 6 };
	SetConsoleScreenBufferSize(console_buffer, win_coords);

	rect.Right = win_coords.X - 1;
	rect.Bottom = win_coords.Y - 1;
	SetConsoleWindowInfo(console_buffer, TRUE, &rect);
}

/* 그래픽 가동 시작 */
void Graphic::start()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(console_buffer, &cursorInfo);
	SetConsoleActiveScreenBuffer(console_buffer);

	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i] = std::thread(&Graphic::render_bullet, this);
}

/* 그래픽 가동 중지 */
void Graphic::stop()
{
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i].~thread();
}

void Graphic::draw(COORD pos, char value, COLOR color, COLOR bgcolor)
{
	DWORD dw;

	mtx_console_buffer.lock();

		SetConsoleTextAttribute(console_buffer, (bgcolor << 4) | color);
		SetConsoleCursorPosition(console_buffer, pos);
		WriteConsoleA(console_buffer, &value, 1, &dw, NULL);

	mtx_console_buffer.unlock();
}

void Graphic::draw(COORD pos, const char* value, COLOR color, COLOR bgcolor)
{
	DWORD dw;

	mtx_console_buffer.lock();

		SetConsoleTextAttribute(console_buffer, (bgcolor << 4) | color);
		SetConsoleCursorPosition(console_buffer, pos);
		WriteConsoleA(console_buffer, value, strlen(value), &dw, NULL);

	mtx_console_buffer.unlock();
}

/* 사용 대기 스킬 출력 처리 */
void Graphic::render_bullet()
{
	while (1)
		fire_queue.Dequeue().fire();
}

/* 경기장 화면 초기화 */
void Graphic::clear_frame()
{
	
}

/* 경기장 그리기 */
void Graphic::draw_field()
{
	char buff[600] = "";

	/* 윗변 그리기 */

	for (int i = 0; i < 2 * field_width + 1; i++)
		buff[i] = ':';
	buff[2 * field_width + 2] = NULL;
	draw(COORD{ field.Left, field.Top }, buff, GREEN, GREEN);

	/* 왼쪽, 오른쪽 변 그리기 */

	for (SHORT i = field.Top + 1; i < field.Bottom; i++)
	{
		draw(COORD{ field.Left, i }, ':', GREEN, GREEN);
		draw(COORD{ field.Left + 2 * field_width, i }, ':', GREEN, GREEN);
	}

	/* 아랫변 그리기 */
	
	for (int i = 0; i < 2 * field_width + 1; i++)
		buff[i] = ':';
	
	buff[2 * field_width + 2] = NULL;
	draw(COORD{ field.Left, field.Bottom }, buff, GREEN, GREEN);
}