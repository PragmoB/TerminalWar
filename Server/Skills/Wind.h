#pragma once

#include "ActiveSkill.h"

class Wind : public ActiveSkill
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
	virtual int get_ordinal() const;

	virtual bool upgradable() const;
	virtual bool upgradable_to(SKILL_TYPE type) const;
	virtual bool downgradable() const;
	virtual bool downgradable_to(SKILL_TYPE type) const;
};