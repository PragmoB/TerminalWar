#pragma once

#include "PassiveSkill.h"

class WindStep : public PassiveSkill
{
private:
	static const int MAX_LEVEL;
	static const int SPEED_RATE[];
public:
	WindStep(Player* owner, int level);
	virtual int get_speed_rate();

	virtual SKILL_TYPE get_type() const;
	virtual int get_max_level() const;
	virtual int get_ordinal() const;

	virtual bool upgradable() const;
	virtual bool upgradable_to(SKILL_TYPE type) const;
	virtual bool downgradable() const;
	virtual bool downgradable_to(SKILL_TYPE type) const;

	// 스킬 강화 메시지 구성
	virtual void get_level_up_message(char* output, int len) const;
	// 스킬 습득 메시지 구성
	virtual void get_learn_message(char* output, int len) const;
	virtual const char* get_skill_name() const;
};