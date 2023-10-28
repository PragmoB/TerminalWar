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
	bool me;

public:
	Player(COORD pos, int HP, char chracter, bool me);
	// ���� ��ġ�� ����� �巯��
	void appear();
	// ���� ��ġ���� ����� �����
	void disappear();
	void move(DIRECTION dir);
	void shoot(DIRECTION dir);
	void hit();

	bool is_me();
	~Player();
};