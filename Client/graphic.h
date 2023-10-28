#pragma once

#include "protocol.h"
#include "blocking_queue.h"
#include "Bullet.h"

#include <thread>
#include <mutex>

/*
 * �������� ����ϴ� �׷��� ���� ���Ǻ�.
 * ���� �׷��� ���� �۾��� �����
 */
enum COLOR {
	BLACK = 0, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE, DARK_RED,
	DARK_PURPLE, DARK_YELLOW, GRAY, DARK_GRAY, BLUE, GREEN, SKY_BLUE,
	RED, PURPLE, YELLOW, WHITE
};
class Graphic
{
private:
	bool started;

	// �׷��� ��ü�� ���� �ܼ� ȭ��
	HANDLE console_buffer;
	// console_buffer ����ȭ ��� ���ؽ�
	std::mutex mtx_console_buffer;
	std::thread bullet_renderers[10];

	/* ��� ��� ��ų ��� ó�� */
	void render_bullet();

	
public:
	BlockingQueue<Bullet> fire_queue;
	
	Graphic();
	
	bool is_started() const;

	/* pos��ġ�� color�������� value��� */
	void draw(COORD pos, char value, COLOR color = GRAY, COLOR bgcolor = BLACK);
	void draw(COORD pos, const char* value, COLOR color = GRAY, COLOR bgcolor = BLACK);

	/* �׷��� ���� ���� */
	void start();

	/* �׷��� ���� ���� */
	void stop();

	/* ����� ȭ�� �ʱ�ȭ */
	void clear_frame();

	/* ����� �׸��� */
	void draw_field();
};