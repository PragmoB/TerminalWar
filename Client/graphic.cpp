#include <iostream>
#include <windows.h>

#include "blocking_queue.h"

#include "Graphic.h"
#include "interface.h"
#include "protocol.h"
#include "Bullet.h"

using namespace std;

// ���� �׷��� ����
Graphic graphic;

Graphic::Graphic() : fire_queue(50), started(false)
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

	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i] = std::thread(&Graphic::render_bullet, this);
}

bool Graphic::is_started() const
{
	return started;
}

/* �׷��� ���� ���� */
void Graphic::start()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = FALSE; //Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(console_buffer, &cursorInfo);
	SetConsoleActiveScreenBuffer(console_buffer);

	started = true;
}

/* �׷��� ���� ���� */
void Graphic::stop()
{
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));

	started = false;
}

void Graphic::draw(COORD pos, char value, COLOR color, COLOR bgcolor)
{
	char temp[2] = { value, NULL };
	draw(pos, temp, color, bgcolor);
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

/* ��� ��� ��ų ��� ó�� */
void Graphic::render_bullet()
{
	while (1)
		fire_queue.Dequeue().fire();
}

/* ����� ȭ�� �ʱ�ȭ */
void Graphic::clear_frame()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	GetConsoleScreenBufferInfo(console_buffer, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(console_buffer, ' ', dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(console_buffer, coordScreen);
}

/* ����� �׸��� */
void Graphic::draw_field()
{
	char buff[600] = "";

	/* ���� �׸��� */

	for (int i = 0; i < 2 * field_width + 1; i++)
		buff[i] = ':';
	buff[2 * field_width + 2] = NULL;
	draw(COORD{ field.Left, field.Top }, buff, GREEN, GREEN);

	/* ����, ������ �� �׸��� */

	for (SHORT i = field.Top + 1; i < field.Bottom + 1; i++)
	{
		draw(COORD{ field.Left, i }, ':', GREEN, GREEN);
		draw(COORD{ field.Left + 2 * field_width, i }, ':', GREEN, GREEN);
	}

	/* �Ʒ��� �׸��� */
	
	for (int i = 0; i < 2 * field_width + 1; i++)
		buff[i] = ':';
	
	buff[2 * field_width + 2] = NULL;
	draw(COORD{ field.Left, field.Bottom + 1 }, buff, GREEN, GREEN); // field.Bottom + 1 => ü�� ���� ǥ������ ���
}