#pragma once

#include <windows.h>

#include "interface.h"
#include "Skills/Skill.h"

class Wind : public Skill
{
public:
	static const int MAX_LEVEL;
	static const int DAMAGE[];
	static const int COOLDOWN[];
	static const int BPS[];
	static const int DISTANCE[];
public:
	Wind(Player* owner, int level = 1);
	virtual bool cast(DIRECTION dir);

	virtual int get_damage() const;
	virtual int get_cooldown() const;
	virtual int get_bps() const;
	virtual int get_distance() const;
	virtual SKILL_TYPE get_type() const;
	virtual int get_max_level() const;
	virtual const char* get_skill_name() const;
	virtual void get_level_up_message(char* output, int len) const;
	virtual void get_learn_message(char* output, int len) const;
	virtual int get_ordinal() const;
	virtual bool upgradable() const;
	virtual bool upgradable_to(SKILL_TYPE type) const;
};