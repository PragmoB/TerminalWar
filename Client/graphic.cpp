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

void gotoxy(SHORT x, SHORT y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Graphic::Graphic() : fire_queue(50)
{	}

/* �׷��� ���� ���� */
void Graphic::start()
{
	frame_loader = std::thread(&Graphic::print_frame, this);
	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i] = std::thread(&Graphic::render_bullet, this);
}

/* �׷��� ���� ���� */
void Graphic::stop()
{
	frame_loader.~thread();
	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i].~thread();
}

/* ������ ������Ʈ */
void Graphic::print_frame()
{
	while (1)
	{
		gotoxy(0, 0);

		for (uint32_t i = 0; i < FRAME_Y; i++)
			cout << frame[i] << endl;

		Sleep(20);
	}
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
	for (uint32_t i = 0; i < FRAME_Y; i++)
	{
		for (LONG j = 0; j < field.right + 2; j++)
			frame[i][j] = ' ';

		frame[i][field.right + 2 - 1] = NULL;
	}
}

/* ����� �׸��� */
void Graphic::draw_field()
{
	//uint32_t field_width = field.right - field.left;

	/* ���� �׸��� */

	for (uint32_t i = 0; i < field_width * 2; i++)
		frame[field.top][field.left + 2 + i] = (i % 2) ? '\xa1' : '\xa6';

	frame[field.top][field.left] = '\xa6';	frame[field.top][field.left + 1] = '\xa3';
	frame[field.top][field.left + 2 + field_width * 2] = '\xa6';
	frame[field.top][field.left + 2 + field_width * 2 + 1] = '\xa4';
	frame[field.top][field.left + 2 + field_width * 2 + 2] = NULL;

	/* ����, ������ �� �׸��� */

	for (LONG i = field.top + 1; i < field.bottom; i++)
	{
		frame[i][field.left] = '\xa6';
		frame[i][field.left + 1] = '\xa2';

		frame[i][field.right] = '\xa6';
		frame[i][field.right + 1] = '\xa2';
	}

	/* �Ʒ��� �׸��� */
	
	for (uint32_t i = 0; i < field_width * 2; i++)
		frame[field.bottom][field.left + 2 + i] = (i % 2) ? '\xa1' : '\xa6';
	frame[field.bottom][field.left] = '\xa6';
	frame[field.bottom][field.left + 1] = '\xa6';
	frame[field.bottom][field.left + 2 + field_width * 2] = '\xa6';
	frame[field.bottom][field.left + 2 + field_width * 2 + 1] = '\xa5';
	frame[field.bottom][field.left + 2 + field_width * 2 + 2] = NULL;
}