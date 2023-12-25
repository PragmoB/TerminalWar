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

// ���� �׷��� ����
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

void Graphic::draw_in_field(COORD pos, const char* value, COLOR color, COLOR bgcolor)
{
	/* ���ι��� �˻� */

	if ((signed)pos.Y <= FIELD.Top || FIELD.Bottom < (signed)pos.Y)
		return;

	/* ����� ���� ����� �ʰ� �ϴ� value ��� ���� ���� (���ι��� �˻�) */

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

	/* [start, end]�� ��� ������ ���߾� ��� ���� */

	if (end == len_value) // value�� ������ ��� ������ ������ �ʿ䰡 ���ٸ�
		draw(COORD{ pos.X + start, pos.Y }, value + start, color, bgcolor); // �ٷ� ���
	else
	{
		char str[200] = "";
		for (int i = 0; i < end - start; i++)
			str[i] = value[start + i];
		str[end - start] = NULL;

		draw(COORD{ pos.X + start, pos.Y }, str, color, bgcolor);
	}
	
}

/* ��� ��� ��ų ��� ó�� */
void Graphic::render_skill()
{
	while (1)
	{
		SkillParam skill_param = skill_queue.Dequeue();
		skill_param.skill->cast(skill_param.dir);
	}
}

/* ȭ�� ���ҽ� ��� ����ȭ */
void Graphic::render_resources()
{
	// players, items ��ü�� �ʱ�ȭ�� ��ٸ�
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
void Graphic::draw_field(const char* message)
{
	char buff[600] = "";

	/* ���� �׸��� */

	for (int i = 0; i < 2 * FIELD_WIDTH + 3; i++)
		buff[i] = ':';
	buff[2 * FIELD_WIDTH + 3] = NULL;
	draw(COORD{ FIELD.Left - 1, FIELD.Top }, buff, FIELD_FRAME_COLOR, FIELD_FRAME_COLOR);

	/* ����, ������ �� �׸��� */

	for (SHORT i = FIELD.Top + 1; i < FIELD.Bottom + 1; i++)
	{
		draw(COORD{ FIELD.Left - 1, i }, "::", FIELD_FRAME_COLOR, FIELD_FRAME_COLOR);
		draw(COORD{ FIELD.Left + 2 * FIELD_WIDTH, i }, "::", FIELD_FRAME_COLOR, FIELD_FRAME_COLOR);
	}

	/* �Ʒ��� �׸��� */
	
	for (int i = 0; i < 2 * FIELD_WIDTH + 3; i++)
		buff[i] = ':';
	
	buff[2 * FIELD_WIDTH + 3] = NULL;
	draw(COORD{ FIELD.Left - 1, FIELD.Bottom + 1 }, buff, FIELD_FRAME_COLOR, FIELD_FRAME_COLOR); // FIELD.Bottom + 1 => ü�� ���� ǥ������ ���

	/* ��� ä��� */
	
	for (SHORT i = 0; i < 2 * FIELD_WIDTH - 1; i++)
		buff[i] = ':';
	buff[2 * FIELD_WIDTH - 1] = NULL;
	for (SHORT i = FIELD.Top + 1; i < FIELD.Bottom + 1; i++)
		graphic.draw(COORD{ FIELD.Left + 1, i }, buff, FIELD_BACKGROUND_COLOR, FIELD_BACKGROUND_COLOR);

	/* �޽��� �׷��ֱ� */

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
	server_pos.X *= 2; // ���ι��� �̵��� 2ĭ������ �����
	server_pos.X += FIELD.Left - 1;
	server_pos.Y += FIELD.Top;
	return server_pos;
}
COORD Graphic::get_server_pos_by_client_pos(COORD client_pos)
{
	client_pos.X -= FIELD.Left - 1;
	client_pos.X /= 2; // ���ι��� �̵��� 2ĭ������ �����
	client_pos.Y -= FIELD.Top;
	return client_pos;
}