#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "protocol.h"
#include "Skills/Skill.h"

class Player
{
private:
	COORD pos;
	int HP;
	const char chracter;
	bool me;

	int len_skills;
	Skill* skills[MAX_ACTIVE_SKILL];

	Skill* get_skill(SKILL_TYPE);
public:
	Player(COORD pos, int HP, char chracter, bool me, int len_skills, SKILL_TYPE skills[MAX_ACTIVE_SKILL]);
	// 현재 위치에 모습을 드러냄
	void appear();
	// 현재 위치에서 모습이 사라짐
	void disappear();
	void move(DIRECTION dir);
	void cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	void upgrade_skill(SKILL_TYPE before, SKILL_TYPE after);
	// player를 skill_type으로 공격
	void attack(Player* player, SKILL_TYPE skill_type);
	// skill에 맞는 동작
	void hit(const Skill* skill);

	bool is_me();

	COORD get_pos() const;
	~Player();
};