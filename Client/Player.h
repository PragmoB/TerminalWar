#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "protocol.h"

class Player
{
private:
	COORD pos;
	int HP;
	const char chracter;

public:
	Player(COORD pos, int HP, char chracter);
	// 현재 위치에 모습을 드러냄
	void appear();
	// 현재 위치에서 모습이 사라짐
	void disappear();
	void move(DIRECTION dir);
	void shoot(DIRECTION dir);
	void hit();
	~Player();
};