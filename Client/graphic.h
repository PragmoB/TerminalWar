#pragma once

#include "protocol.h"
#include "blocking_queue.h"
#include "Bullet.h"

#include <thread>

class Graphic
{
private:
	std::thread frame_loader;
	std::thread bullet_renderers[10];

	/* ������ ������Ʈ */
	void print_frame();

	/* ��� ��� ��ų ��� ó�� */
	void render_bullet();
	
public:
	char frame[FRAME_Y][FRAME_X];
	BlockingQueue<Bullet> fire_queue;
	
	Graphic();

	/* �׷��� ���� ���� */
	void start();

	/* �׷��� ���� ���� */
	void stop();

	/* ����� ȭ�� �ʱ�ȭ */
	void clear_frame();

	/* ����� �׸��� */
	void draw_field();
};