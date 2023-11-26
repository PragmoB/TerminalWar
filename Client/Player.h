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
	// 이동(move)중인가? 플래그
	bool moving = false;

	int len_active_skills;
	Skill* active_skills[MAX_ACTIVE_SKILL];

public:
	Player(COORD pos, int HP, char chracter, bool me, int len_active_skills, SKILL_TYPE skills[MAX_ACTIVE_SKILL], int skill_levels[MAX_ACTIVE_SKILL]);
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
	// item을 획득하여 흡수함
	void earn_item(Item* item);

	bool is_me();
	bool is_moving();

	Skill* get_active_skill(SKILL_TYPE skill_type);
	COORD get_pos() const;
	~Player();
};