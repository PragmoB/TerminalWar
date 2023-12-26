#pragma once

#define _WINSOCKAPI_
#include <windows.h>
#include <vector>

#include "graphic.h"

template <typename T_ELEMENT>
class Menu
{
private:
	static const int WIDTH;

	// 콘솔창 안에서 메뉴의 위치(왼쪽 위 끝 점 기준)
	const COORD pos;
	// 커서의 y방향 위치 값(맨 위 = 0, 그 아래 = 1, ...)
	int focus = 0;
	// 커서의 보임/숨김 여부
	bool cursor_is_visible;
	
	// 메뉴의 제목
	const char* title;
	// 글자 수가 가장 많은 항목의 글자 수
	int max_contents_len = 0;

	typedef struct
	{
		// 항목의 값
		T_ELEMENT value;
		// 항목의 다음 선택 가능 시간
		clock_t next_selectable_time;
	} Element;
	std::vector<Element> elements;
private:
	// 선택 쿨타임 표시
	void render_cooldown();
	void set_max_contents_len(int max_contents_len);
public:
	Menu(COORD pos, const char* title, bool cursor_is_visible);
	// 콘솔 창에 모습을 드러냄
	void appear();
	// 콘솔 창에서 모습을 감춤
	void disappear();

	int get_focus();
	bool set_focus(int focus);
	int get_num_contents();
	int get_max_contents_len();
	// focus가 가리키는 요소값 반환
	T_ELEMENT get_element();
	// element에 focus를 맞춤
	bool set_focus(T_ELEMENT element);
	// element의 선택 cooldown(밀리세컨드) 설정
	bool set_cooldown(T_ELEMENT element, int cooldown);

	// 메뉴 항목에 content 추가
	void insert_menu(const char* content, T_ELEMENT element);
	// focus가 가리키는 메뉴 항목을 x번째 글자부터 color색상의 content로 업데이트 함
	// 메뉴 항목의 색상을 다채롭게 구성하기 위한 묘수
	bool update_menu(int focus, SHORT x, const char* content, COLOR color = GRAY);
	bool update_menu(T_ELEMENT element, SHORT x, const char* content, COLOR color = GRAY);
	bool update_menu(T_ELEMENT element, T_ELEMENT element_after);

	// 초기 상태로 돌리기
	void clear();
};

#include "Menu.hpp"