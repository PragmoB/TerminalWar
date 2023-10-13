#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "protocol.h"

class Player
{
private:
	COORD pos;
	const char chracter;

public:
	Player(COORD pos, char chracter);
	void move(DIRECTION dir);
	void shoot(DIRECTION dir);
	void hit();
	~Player();
};