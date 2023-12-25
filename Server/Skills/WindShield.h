#pragma once

#include "Skills/WindStep.h"

class WindShield : public WindStep
{
private:
	static const int MAX_LEVEL;
	static const int SPEED_RATE[];
	static const int DEFENSE_RATE[];
	static const int EVASION_RATE[];
public:
	WindShield(Client* owner, int level = 1);
	virtual int get_speed_rate();
	virtual int get_defense_rate();
	virtual int get_evasion_rate();

	virtual SKILL_TYPE get_type() const;
	virtual int get_max_level() const;
	virtual int get_ordinal() const;

	virtual bool upgradable() const;
	virtual bool upgradable_to(SKILL_TYPE type) const;
	virtual bool downgradable() const;
	virtual bool downgradable_to(SKILL_TYPE type) const;
};