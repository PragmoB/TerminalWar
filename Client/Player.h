#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "protocol.h"
#include "Skills/ActiveSkill.h"
#include "Skills/PassiveSkill.h"

class Player
{
private:
	int speed_increase_rate = 0;
	int defense_rate = 0;
	int damage_increase_rate = 0;
	int evasion_rate = 0;
	int mov_attack_damage = 0;
	int mov_attack_range = 0;
private:
	COORD pos;
	int HP;
	const char chracter;
	bool me;
	// 이동(move)중인가? 플래그
	bool moving = false;

	int len_skills;
	Skill* skills[MAX_SKILL];

public:
	Player(COORD pos, int HP, char chracter, bool me, int len_skills, SKILL_TYPE skills[MAX_SKILL], int skill_levels[MAX_SKILL]);
	// 현재 위치에 모습을 드러냄
	void appear();
	// 현재 위치에서 모습이 사라짐
	void disappear();
	void move(DIRECTION dir);
	void move(COORD pos);
	void cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	void upgrade_skill(SKILL_TYPE before, SKILL_TYPE after);
	// player를 skill_type으로 공격
	void attack(Player* player, SKILL_TYPE skill_type, bool evaded = false);
	// skill에 맞는 동작
	void hit(const ActiveSkill* skill, bool evaded = false);
	void hit_mov_attack(const Player* attacker, bool evaded = false);
	// item을 획득하여 흡수함
	void earn_item(Item* item);

	bool is_me();
	bool is_moving();

	int get_damage_increase_rate() const;
	Skill* get_skill(SKILL_TYPE skill_type);
	COORD get_pos() const;
	int get_mov_attack_damage() const;
	~Player();
};