#pragma once

#include <windows.h>

#include "graphic.h"

class Menu
{
private:
	// 콘솔창 안에서 메뉴의 위치(왼쪽 위 끝 점 기준)
	const COORD pos;
	// 커서의 y방향 위치 값(맨 위 = 0, 그 아래 = 1, ...)
	int focus;
	// 커서의 보임/숨김 여부
	bool cursor_is_visible;
	
	// 메뉴의 제목
	const char* title;
	// 메뉴 항목의 개수
	int num_contents;
	// 글자 수가 가장 많은 항목의 글자 수
	int max_contents_len;

public:
	Menu(COORD pos, const char* title, bool cursor_is_visible);
	// 콘솔 창에 모습을 드러냄
	void appear();
	// 콘솔 창에서 모습을 감춤
	void disappear();

	int get_focus();
	int get_num_contents();
	bool set_focus(int focus);

	// 메뉴 항목에 content 추가
	void insert_menu(const char* content);
	// focus가 가리키는 메뉴 항목을 x번째 글자부터 color색상의 content로 업데이트 함
	// 메뉴 항목의 색상을 다채롭게 구성하기 위한 묘수
	bool update_menu(int focus, SHORT x, const char* content, COLOR color = GRAY);

	// 초기 상태로 돌리기
	void clear();
};