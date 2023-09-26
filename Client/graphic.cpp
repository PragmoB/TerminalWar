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

void gotoxy(SHORT x, SHORT y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Graphic::Graphic() : fire_queue(50)
{	}

/* 그래픽 가동 시작 */
void Graphic::start()
{
	frame_loader = std::thread(&Graphic::print_frame, this);
	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i] = std::thread(&Graphic::render_bullet, this);
}

/* 그래픽 가동 중지 */
void Graphic::stop()
{
	frame_loader.~thread();
	for (uint32_t i = 0; i < 10; i++)
		bullet_renderers[i].~thread();
}

/* 프레임 업데이트 */
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

/* 사용 대기 스킬 출력 처리 */
void Graphic::render_bullet()
{
	while (1)
		fire_queue.Dequeue().fire();
}

/* 경기장 화면 초기화 */
void Graphic::clear_frame()
{
	for (uint32_t i = 0; i < FRAME_Y; i++)
	{
		for (LONG j = 0; j < field.right + 2; j++)
			frame[i][j] = ' ';

		frame[i][field.right + 2 - 1] = NULL;
	}
}

/* 경기장 그리기 */
void Graphic::draw_field()
{
	//uint32_t field_width = field.right - field.left;

	/* 윗변 그리기 */

	for (uint32_t i = 0; i < field_width * 2; i++)
		frame[field.top][field.left + 2 + i] = (i % 2) ? '\xa1' : '\xa6';

	frame[field.top][field.left] = '\xa6';	frame[field.top][field.left + 1] = '\xa3';
	frame[field.top][field.left + 2 + field_width * 2] = '\xa6';
	frame[field.top][field.left + 2 + field_width * 2 + 1] = '\xa4';
	frame[field.top][field.left + 2 + field_width * 2 + 2] = NULL;

	/* 왼쪽, 오른쪽 변 그리기 */

	for (LONG i = field.top + 1; i < field.bottom; i++)
	{
		frame[i][field.left] = '\xa6';
		frame[i][field.left + 1] = '\xa2';

		frame[i][field.right] = '\xa6';
		frame[i][field.right + 1] = '\xa2';
	}

	/* 아랫변 그리기 */
	
	for (uint32_t i = 0; i < field_width * 2; i++)
		frame[field.bottom][field.left + 2 + i] = (i % 2) ? '\xa1' : '\xa6';
	frame[field.bottom][field.left] = '\xa6';
	frame[field.bottom][field.left + 1] = '\xa6';
	frame[field.bottom][field.left + 2 + field_width * 2] = '\xa6';
	frame[field.bottom][field.left + 2 + field_width * 2 + 1] = '\xa5';
	frame[field.bottom][field.left + 2 + field_width * 2 + 2] = NULL;
}