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

	int len_active_skills;
	Skill* active_skills[MAX_ACTIVE_SKILL];

	Skill* get_active_skill(SKILL_TYPE skill_type);
public:
	Player(COORD pos, int HP, char chracter, bool me, int len_active_skills, SKILL_TYPE skills[MAX_ACTIVE_SKILL]);
	// ���� ��ġ�� ����� �巯��
	void appear();
	// ���� ��ġ���� ����� �����
	void disappear();
	void move(DIRECTION dir);
	void cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	void upgrade_skill(SKILL_TYPE before, SKILL_TYPE after);
	// player�� skill_type���� ����
	void attack(Player* player, SKILL_TYPE skill_type);
	// skill�� �´� ����
	void hit(const Skill* skill);

	bool is_me();

	COORD get_pos() const;
	~Player();
};