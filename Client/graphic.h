#pragma once

#include <windows.h>
#include <thread>
#include <mutex>

#include "protocol.h"
#include "blocking_queue.h"
#include "Skills/Skill.h"


/*
 * 전역에서 사용하는 그래픽 엔진 정의부.
 * 게임 그래픽 관련 작업을 담당함
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

	// 그래픽 객체가 쓰는 콘솔 화면
	HANDLE console_buffer;
	// console_buffer 동기화 담당 뮤텍스
	std::mutex mtx_console_buffer;

	std::thread skill_renderers[10];
	std::thread resource_renderer;

	/* 사용 대기 스킬 출력 처리 */
	void render_skill();

	/* 화면 리소스 출력 동기화 */
	void render_resources();
	
public:
	
	Graphic();
	
	bool is_started() const;

	/* pos위치에 color색상으로 value출력 */
	void draw(COORD pos, char value, COLOR color = GRAY, COLOR bgcolor = BLACK);
	void draw(COORD pos, const char* value, COLOR color = GRAY, COLOR bgcolor = BLACK);

	/* 경기장 안을 벗어나지 않는 범위에서 draw한다 */
	void draw_in_field(COORD pos, const char* value, COLOR color = GRAY, COLOR bgcolor = BLACK);

	/* 그래픽 가동 시작 */
	void start();

	/* 그래픽 가동 중지 */
	void stop();

	/* 경기장 화면 초기화 */
	void clear_frame();

	/* 경기장 그리기 */
	void draw_field();

	/* 스킬 사용 요청 */
	void cast_skill(Skill* skill, DIRECTION dir);

	/* 서버의 좌표값을 클라이언트 좌표(화면상 좌표)로 바꿈 */
	COORD get_client_pos_by_server_pos(COORD server_pos);
};