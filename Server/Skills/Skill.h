#pragma once

#include <windows.h>
#include <ctime>

#include "protocol.h"

class Client;

class Skill
{
private:
	int level;

	// ���� ��ų ��� ���ɽð�
	clock_t next_able_time = 0;
protected:
	int damage;
	int cooldown;
	Client* owner;
public:
	const int MAX_LEVEL;
	const SKILL_TYPE type;

	Skill(Client* owner, int level, SKILL_TYPE type, int MAX_LEVEL);

	virtual bool cast(DIRECTION dir);
	virtual void level_up();
	int get_level() const;
	int get_damage() const;
	int get_cooldown() const;
	Client* get_owner() const;
};