#pragma once

#include "Skills/Skill.h"


class LightningStrike : public Skill
{
public:
	static const int MAX_LEVEL;
	static const int DAMAGE[];
	static const int COOLDOWN[];
public:

	LightningStrike(Player* owner, int level);

	// dir 방향으로 스킬시전
	virtual bool cast(DIRECTION dir);

	virtual int get_damage() const;
	virtual int get_cooldown() const;
	virtual SKILL_TYPE get_type() const;
	virtual int get_max_level() const;
	virtual const char* get_skill_name() const;
	// 스킬 강화 메시지 구성
	virtual void get_level_up_message(char* output, int len) const;
	// 스킬 습득 메시지 구성
	virtual void get_learn_message(char* output, int len) const;
	virtual int get_ordinal() const;

	virtual bool upgradable() const;
	virtual bool upgradable_to(SKILL_TYPE type) const;
};