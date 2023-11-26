#pragma once

#include <windows.h>
#include <thread>
#include <mutex>

#include "protocol.h"
#include "blocking_queue.h"
#include "Skills/Skill.h"


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
	Skill* skill;
	DIRECTION dir;
} SkillParam;

class Graphic
{
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

	/* ����� �׸��� */
	void draw_field();

	/* ��ų ��� ��û */
	void cast_skill(Skill* skill, DIRECTION dir);

	/* ������ ��ǥ���� Ŭ���̾�Ʈ ��ǥ(ȭ��� ��ǥ)�� �ٲ� */
	COORD get_client_pos_by_server_pos(COORD server_pos);
};