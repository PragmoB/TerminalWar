#pragma once

#include <windows.h>
#include <ctime>

#include "protocol.h"

class Player;

class Skill
{
private:
	int level;

	// ���� ��ų ��� ���ɽð�
	clock_t next_able_time = 0;
protected:
	int damage;
	int cooldown;
public:
	const int MAX_LEVEL;
	const Player* owner;
	const SKILL_TYPE type;

	Skill(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL);
	
	// dir �������� ��ų����
	virtual bool cast(DIRECTION dir);
	// Player�� ����
	virtual void attack(Player* player);
	virtual void level_up();
	int get_level() const;
	int get_damage() const;
	int get_cooldown() const;
};