#include <iostream>
#include <windows.h>

#include <map>
#include <list>

#include "blocking_queue.h"

#include "Graphic.h"
#include "Values/interface.h"
#include "protocol.h"

#include "Player.h"
#include "Items/Item.h"

using namespace std;

// 게임 그래픽 엔진
Graphic graphic;

extern map<DWORD, Player*> players;
extern list<Item*> items;

const COLOR Graphic::FIELD_FRAME_COLOR = GREEN;
const COLOR Graphic::FIELD_BACKGROUND_COLOR = BLACK;

Graphic::Graphic() : skill_queue(50), started(false)
{
	console_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SMALL_RECT rect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(console_buffer, TRUE, &rect);

	COORD win_coords = { FIELD.Left + 2 * FIELD_WIDTH + 45, FIELD.Bottom + 6 };
	SetConsoleScreenBufferSize(console_buffer, win_coords);

	rect.Right = win_coords.X - 1;
	rect.Bottom = win_coords.Y - 1;
	SetConsoleWindowInfo(console_buffer, TRUE, &rect);

	for (uint32_t i = 0; i < 10; i++)
		skill_renderers[i] = std::thread(&Graphic::render_skill, this);

	resource_renderer = std::thread(&Graphic::render_resources, this);
}

bool Graphic::is_started() const
{
	return started;
}

/* 그래픽 가동 시작 */
void Graphic::start()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(console_buffer, &cursorInfo);
	SetConsoleActiveScreenBuffer(console_buffer);

	started = true;
}

/* 그래픽 가동 중지 */
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

void Graphic::draw_in_field(COORD pos, const char* value, COLOR color, COLOR bgcolor)
{
	/* 세로방향 검사 */

	if ((signed)pos.Y <= FIELD.Top || FIELD.Bottom < (signed)pos.Y)
		return;

	/* 경기장 안을 벗어나지 않게 하는 value 출력 범위 결정 (가로방향 검사) */

	const size_t len_value = strlen(value);

	SHORT start = FIELD.Left - pos.X + 1;
	if (start < 0)
		start = 0;
	if (len_value <= start)
		return;

	SHORT end =  - ((signed)len_value + pos.X - (FIELD.Left + 2 * FIELD_WIDTH));
	if (end > 0)
		end = 0;
	end += len_value;
	if (end < 0)
		return;

	/* [start, end]의 출력 범위에 맞추어 출력 수행 */

	if (end == len_value) // value의 오른편 출력 범위를 제한할 필요가 없다면
		draw(COORD{ pos.X + start, pos.Y }, value + start, color, bgcolor); // 바로 출력
	else
	{
		char str[200] = "";
		for (int i = 0; i < end - start; i++)
			str[i] = value[start + i];
		str[end - start] = NULL;

		draw(COORD{ pos.X + start, pos.Y }, str, color, bgcolor);
	}
	
}

/* 사용 대기 스킬 출력 처리 */
void Graphic::render_skill()
{
	while (1)
	{
		SkillParam skill_param = skill_queue.Dequeue();
		skill_param.skill->cast(skill_param.dir);
	}
}

/* 화면 리소스 출력 동기화 */
void Graphic::render_resources()
{
	// players, items 객체의 초기화를 기다림
	Sleep(5000);

	while (1)
	{
		for (map<DWORD, Player*>::iterator iter = players.begin();
			iter != players.end(); iter++)
		{
			if (!iter->second->is_moving())
				iter->second->appear();
		}

		for (list<Item*>::iterator iter = items.begin();
			iter != items.end(); iter++)
			(*iter)->appear();

		Sleep(700);
	}
}

/* 경기장 화면 초기화 */
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

/* 경기장 그리기 */
void Graphic::draw_field(const char* message)
{
	char buff[600] = "";

	/* 윗변 그리기 */

	for (int i = 0; i < 2 * FIELD_WIDTH + 3; i++)
		buff[i] = ':';
	buff[2 * FIELD_WIDTH + 3] = NULL;
	draw(COORD{ FIELD.Left - 1, FIELD.Top }, buff, FIELD_FRAME_COLOR, FIELD_FRAME_COLOR);

	/* 왼쪽, 오른쪽 변 그리기 */

	for (SHORT i = FIELD.Top + 1; i < FIELD.Bottom + 1; i++)
	{
		draw(COORD{ FIELD.Left - 1, i }, "::", FIELD_FRAME_COLOR, FIELD_FRAME_COLOR);
		draw(COORD{ FIELD.Left + 2 * FIELD_WIDTH, i }, "::", FIELD_FRAME_COLOR, FIELD_FRAME_COLOR);
	}

	/* 아랫변 그리기 */
	
	for (int i = 0; i < 2 * FIELD_WIDTH + 3; i++)
		buff[i] = ':';
	
	buff[2 * FIELD_WIDTH + 3] = NULL;
	draw(COORD{ FIELD.Left - 1, FIELD.Bottom + 1 }, buff, FIELD_FRAME_COLOR, FIELD_FRAME_COLOR); // FIELD.Bottom + 1 => 체력 상태 표시줄을 고려

	/* 배경 채우기 */
	
	for (SHORT i = 0; i < 2 * FIELD_WIDTH - 1; i++)
		buff[i] = ':';
	buff[2 * FIELD_WIDTH - 1] = NULL;
	for (SHORT i = FIELD.Top + 1; i < FIELD.Bottom + 1; i++)
		graphic.draw(COORD{ FIELD.Left + 1, i }, buff, FIELD_BACKGROUND_COLOR, FIELD_BACKGROUND_COLOR);

	/* 메시지 그려넣기 */

	if (message)
		draw(COORD{ FIELD.Left + 2, FIELD.Top }, message, BLACK, FIELD_FRAME_COLOR);
}

void Graphic::cast_skill(ActiveSkill* skill, DIRECTION dir)
{
	SkillParam skill_param;
	skill_param.skill = skill;
	skill_param.dir = dir;
	skill_queue.Enqueue(skill_param);
}

COORD Graphic::get_client_pos_by_server_pos(COORD server_pos)
{
	server_pos.X *= 2; // 가로방향 이동은 2칸씩임을 고려함
	server_pos.X += FIELD.Left - 1;
	server_pos.Y += FIELD.Top;
	return server_pos;
}
COORD Graphic::get_server_pos_by_client_pos(COORD client_pos)
{
	client_pos.X -= FIELD.Left - 1;
	client_pos.X /= 2; // 가로방향 이동은 2칸씩임을 고려함
	client_pos.Y -= FIELD.Top;
	return client_pos;
}