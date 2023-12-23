#pragma once

#include "Skill.h"

class ActiveSkill : public Skill
{
private:
	// ���� ��ų ��� ���ɽð�
	clock_t next_able_time = 0;
public:
	ActiveSkill(Client* owner, int level);
	bool castable() const;
	virtual bool cast(DIRECTION dir);
	virtual int get_damage() const = 0;
	virtual int get_cooldown() const = 0;
};