#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "protocol.h"

class Player
{
private:
	POINT pos;
	const char chracter;

public:
	Player(POINT pos, char chracter);
	void move(DIRECTION dir);
	void shoot(DIRECTION dir);
	void hit();
	~Player();
};