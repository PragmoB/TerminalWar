#pragma once

#include <iostream>
#include <windows.h>

#include "protocol.h"
#include "Skills/Skill.h"

class Shoot : public Skill
{
private:
	static const int DAMAGE[5]; // [i] = (i + 1)�������� �Ѿ��� ������
	static const int COOLDOWN[5]; // [i] = (i + 1)�������� ���� ���ð�
	static const int BPS[5]; // [i] = (i + 1)�������� �Ѿ��� �ӵ�
	static const int DISTANCE[5]; // [i] = (i + 1)�������� ���ư� �Ÿ�
protected:
	int bps; // the valocity of the bullet. blocks per second
	int distance; // ���ư� �Ÿ�
public:
	Shoot(Client* owner, int level = 1, SKILL_TYPE type = SHOOT, int MAX_LEVEL = 5);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();

	int get_bps();
	int get_distance();
};