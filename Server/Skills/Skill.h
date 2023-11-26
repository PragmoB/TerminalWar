#pragma once

#include <windows.h>
#include <ctime>

#include "interface.h"

class Client;

enum SKILL_TYPE
{
	UNKNOWN,
	SHOOT,
	SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND
};

const SKILL_TYPE SKILL_TYPE_LIST[] = { SHOOT, SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND };

class Skill
{
private:
	int level;

	// 다음 스킬 사용 가능시간
	clock_t next_able_time = 0;
	Client* owner;
public:
	static Skill* create_object_by_type(SKILL_TYPE type, Client* owner);
	static const Skill* get_object_by_type(SKILL_TYPE type);

	Skill(Client* owner, int level);

	virtual bool cast(DIRECTION dir);
	virtual bool level_up();
	int get_level() const;
	Client* get_owner() const;

	virtual int get_damage() const = 0;
	virtual int get_cooldown() const = 0;
	virtual SKILL_TYPE get_type() const = 0;
	virtual int get_max_level() const = 0;
	virtual int get_ordinal() const = 0;

	virtual bool upgradable() const = 0;
	virtual bool upgradable_to(SKILL_TYPE type) const = 0;
	virtual bool downgradable() const = 0;
	virtual bool downgradable_to(SKILL_TYPE type) const = 0;
};