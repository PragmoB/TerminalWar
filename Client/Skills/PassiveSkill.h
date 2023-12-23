#pragma once

#include "Skill.h"

class PassiveSkill : public Skill
{
public:
	PassiveSkill(Player* owner, int level);

	virtual int get_speed_rate();
	virtual int get_defense_rate();
	virtual int get_damage_rate();
	virtual int get_evasion_rate();
};