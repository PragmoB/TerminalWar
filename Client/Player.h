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
	// ���� ��ġ�� ����� �巯��
	void appear();
	// ���� ��ġ���� ����� �����
	void disappear();
	void move(DIRECTION dir);
	void shoot(DIRECTION dir);
	void hit();
	~Player();
};