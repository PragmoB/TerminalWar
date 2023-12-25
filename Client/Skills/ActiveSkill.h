#pragma once

#include "Skill.h"

#include "Direction.h"

class ActiveSkill : public Skill
{
private:
	// ���� ��ų ��� ���ɽð�
	clock_t next_able_time = 0;
public:
	ActiveSkill(Player* owner, int level);

	// dir �������� ��ų����
	virtual bool cast(DIRECTION dir);

	virtual int get_damage() const = 0;
	virtual int get_cooldown() const = 0;
};