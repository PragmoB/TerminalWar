#pragma once

#include <iostream>
#include <windows.h>

#include "protocol.h"
#include "interface.h"

class Bullet
{
private:
	COORD pos; // ���� ��ġ
	DIRECTION dir; // ����
	uint32_t bps; // the valocity of the bullet. blocks per second
	uint32_t distance; // ���ư� �Ÿ�
	char shape_horizontal; // ���η� ���ư� �� �Ѿ��� ���
	char shape_vertical; // ���η� ���ư� �� �Ѿ��� ���
public:
	Bullet() : bps(25), shape_horizontal('-'), shape_vertical('|'), distance(10) { }
	Bullet(COORD pos, DIRECTION dir) : 
		bps(25), shape_horizontal('-'), shape_vertical('|'), distance(10), 
		pos(pos), dir(dir) { }
	void fire();
};