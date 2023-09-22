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

	/* 프레임 업데이트 */
	void print_frame();

	/* 사용 대기 스킬 출력 처리 */
	void render_bullet();
	
public:
	char frame[FRAME_Y][FRAME_X];
	BlockingQueue<Bullet> fire_queue;
	
	Graphic();

	/* 그래픽 가동 시작 */
	void start();

	/* 그래픽 가동 중지 */
	void stop();

	/* 경기장 화면 초기화 */
	void clear_frame();

	/* 경기장 그리기 */
	void draw_field();
};