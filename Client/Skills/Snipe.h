#pragma once

#include "Skills/Skill.h"
#include "Skills/Shoot.h"

class Snipe : public Shoot
{
public :
	static const int MAX_LEVEL;
	static const int DAMAGE[]; // [i] = (i + 1)�������� �Ѿ��� ������
	static const int COOLDOWN[]; // [i] = (i + 1)�������� ���� ���ð�
	static const int BPS[]; // [i] = (i + 1)�������� �Ѿ��� �ӵ�
	static const int DISTANCE[]; // [i] = (i + 1)�������� ���ư� �Ÿ�
public:
	Snipe(Player* owner, int level = 1);
	virtual bool cast(DIRECTION dir);

	virtual int get_damage() const;
	virtual int get_cooldown() const;
	virtual int get_bps() const;
	virtual int get_distance() const;
	virtual SKILL_TYPE get_type() const;
	virtual int get_max_level() const;
	virtual const char* get_skill_name() const;
	virtual void get_level_up_message(char* output, int len) const;
	virtual void get_learn_message(char* output, int len) const;
	virtual int get_ordinal() const;
	virtual bool upgradable() const;
	virtual bool upgradable_to(SKILL_TYPE type) const;
};