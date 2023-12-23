#pragma once

#include <windows.h>
#include <ctime>

#include "interface.h"

class Player;

enum SKILL_TYPE
{
	UNKNOWN,

	// ��Ƽ�� ��ų
	SHOOT, SNIPE,
	SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND,
	LIGHTNING_STRIKE,

	// �нú� ��ų
	SHOES
};
const SKILL_TYPE SKILL_TYPE_LIST[] = { SHOOT, SNIPE, SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND, LIGHTNING_STRIKE, SHOES };

class Skill
{
private:
	int level;
	Player* owner;
public:
	static Skill* create_object_by_type(SKILL_TYPE type, Player* owner, int level = 1);
	static const Skill* get_object_by_type(SKILL_TYPE type);

	Skill(Player* owner, int level);
	
	virtual void level_up();
	
	Player* get_owner() const;
	int get_level() const;
	virtual int get_max_level() const = 0;

	// ��ų ��ȭ �޽��� ����
	virtual void get_level_up_message(char* output, int len) const = 0;
	// ��ų ���� �޽��� ����
	virtual void get_learn_message(char* output, int len) const = 0;
	virtual int get_ordinal() const = 0;

	virtual bool upgradable() const = 0;
	virtual bool upgradable_to(SKILL_TYPE type) const = 0;

	virtual SKILL_TYPE get_type() const = 0;
	virtual const char* get_skill_name() const = 0;
};