#pragma once

#include <iostream>
#include <windows.h>

#include "protocol.h"
#include "interface.h"

class Bullet
{
private:
	uint32_t x, y; // ���� ��ġ
	DIRECTION dir; // ����
	uint32_t bps; // the valocity of the bullet. blocks per second
	uint32_t distance; // ���ư� �Ÿ�
	char shape_horizontal; // ���η� ���ư� �� �Ѿ��� ���
	char shape_vertical; // ���η� ���ư� �� �Ѿ��� ���
public:
	Bullet() : bps(25), shape_horizontal('-'), shape_vertical('|'), distance(10) { }
	Bullet(uint32_t x, uint32_t y, DIRECTION dir) : 
		bps(25), shape_horizontal('-'), shape_vertical('|'), distance(10), 
		x(x), y(y), dir(dir) { }
	void fire(RECT field, char frame[FRAME_Y][FRAME_X] = NULL);
};