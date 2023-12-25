#pragma once

#define _WINSOCKAPI_
#include <windows.h>
#include <ctime>

class Player;

enum SKILL_TYPE
{
	UNKNOWN,

	// 액티브 스킬
	SHOOT, SNIPE,
	SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND,
	LIGHTNING_STRIKE,

	// 패시브 스킬
	WIND_STEP, CELESTIAL_ARROW_STEP, HERMES_STEP, WIND_SHIELD
};

const SKILL_TYPE SKILL_TYPE_LIST[] = {
	SHOOT, SNIPE,
	SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND,
	LIGHTNING_STRIKE,
	
	WIND_STEP, CELESTIAL_ARROW_STEP, HERMES_STEP, WIND_SHIELD
};

class Skill
{
private:
	int level;
	Player* owner;
public:
	static Skill* create_object_by_type(SKILL_TYPE type, Player* owner);
	static const Skill* get_object_by_type(SKILL_TYPE type);

	Skill(Player* owner, int level);

	virtual bool level_up();
	int get_level() const;
	Player* get_owner() const;

	virtual SKILL_TYPE get_type() const = 0;
	virtual int get_max_level() const = 0;
	virtual int get_ordinal() const = 0;

	virtual bool upgradable() const = 0;
	virtual bool upgradable_to(SKILL_TYPE type) const = 0;
	virtual bool downgradable() const = 0;
	virtual bool downgradable_to(SKILL_TYPE type) const = 0;
};