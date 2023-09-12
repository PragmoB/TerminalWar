#pragma once

#include <iostream>
#include <windows.h>

#include "protocol.h"
#include "interface.h"

class Bullet
{
private:
	uint32_t x, y; // 현재 위치
	DIRECTION dir; // 방향
	uint32_t bps; // the valocity of the bullet. blocks per second
	uint32_t distance; // 날아갈 거리
	char shape_horizontal; // 가로로 날아갈 때 총알의 모양
	char shape_vertical; // 세로로 날아갈 때 총알의 모양
public:
	Bullet() : bps(25), shape_horizontal('-'), shape_vertical('|'), distance(10) { }
	Bullet(uint32_t x, uint32_t y, DIRECTION dir) : 
		bps(25), shape_horizontal('-'), shape_vertical('|'), distance(10), 
		x(x), y(y), dir(dir) { }
	void fire(RECT field, char frame[FRAME_Y][FRAME_X] = NULL);
};