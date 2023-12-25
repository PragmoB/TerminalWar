#pragma once

#define _WINSOCKAPI_
#include <windows.h>
#include <thread>
#include <mutex>

#include "protocol.h"
#include "blocking_queue.h"
#include "Skills/ActiveSkill.h"


/*
 * �������� ����ϴ� �׷��� ���� ���Ǻ�.
 * ���� �׷��� ���� �۾��� �����
 */
enum COLOR {
	BLACK = 0, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE, DARK_RED,
	DARK_PURPLE, DARK_YELLOW, GRAY, DARK_GRAY, BLUE, GREEN, SKY_BLUE,
	RED, PURPLE, YELLOW, WHITE
};
typedef struct {
	ActiveSkill* skill;
	DIRECTION dir;
} SkillParam;

class Graphic
{
public :
	static const COLOR FIELD_FRAME_COLOR;
	static const COLOR FIELD_BACKGROUND_COLOR;

private:
	BlockingQueue<SkillParam> skill_queue;
	bool started;

	// �׷��� ��ü�� ���� �ܼ� ȭ��
	HANDLE console_buffer;
	// console_buffer ����ȭ ��� ���ؽ�
	std::mutex mtx_console_buffer;

	std::thread skill_renderers[10];
	std::thread resource_renderer;

	/* ��� ��� ��ų ��� ó�� */
	void render_skill();

	/* ȭ�� ���ҽ� ��� ����ȭ */
	void render_resources();
	
public:
	
	Graphic();
	
	bool is_started() const;

	/* pos��ġ�� color�������� value��� */
	void draw(COORD pos, char value, COLOR color = GRAY, COLOR bgcolor = BLACK);
	void draw(COORD pos, const char* value, COLOR color = GRAY, COLOR bgcolor = BLACK);

	/* ����� ���� ����� �ʴ� �������� draw�Ѵ� */
	void draw_in_field(COORD pos, const char* value, COLOR color = GRAY, COLOR bgcolor = BLACK);

	/* �׷��� ���� ���� */
	void start();

	/* �׷��� ���� ���� */
	void stop();

	/* ����� ȭ�� �ʱ�ȭ */
	void clear_frame();

	/* ����� �׸���, ����� ���� �޽����� ���� �Ѹ� */
	void draw_field(const char* message = NULL);

	/* ��ų ��� ��û */
	void cast_skill(ActiveSkill* skill, DIRECTION dir);

	/* ������ ��ǥ���� Ŭ���̾�Ʈ ��ǥ(ȭ��� ��ǥ)�� �ٲ� */
	COORD get_client_pos_by_server_pos(COORD server_pos);
	/* Ŭ���̾�Ʈ ��ǥ(ȭ��� ��ǥ)���� ������ ��ǥ������ �ٲ� */
	COORD get_server_pos_by_client_pos(COORD client_pos);
};