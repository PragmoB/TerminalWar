#pragma once

#include <windows.h>
#include <ctime>

#include "interface.h"

class Player;

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
	Player* owner;
public:
	static Skill* create_object_by_type(SKILL_TYPE type, Player* owner, int level = 1);
	static const Skill* get_object_by_type(SKILL_TYPE type);

	Skill(Player* owner, int level);
	
	// dir 방향으로 스킬시전
	virtual bool cast(DIRECTION dir);
	virtual void level_up();
	
	Player* get_owner() const;
	int get_level() const;

	virtual int get_damage() const = 0;
	virtual int get_cooldown() const = 0;
	virtual SKILL_TYPE get_type() const = 0;
	virtual int get_max_level() const = 0;
	virtual const char* get_skill_name() const = 0;
	// 스킬 강화 메시지 구성
	virtual void get_level_up_message(char* output, int len) const = 0;
	// 스킬 습득 메시지 구성
	virtual void get_learn_message(char* output, int len) const = 0;
	virtual int get_ordinal() const = 0;

	virtual bool upgradable() const = 0;
	virtual bool upgradable_to(SKILL_TYPE type) const = 0;
};