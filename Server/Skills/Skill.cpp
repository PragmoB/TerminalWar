#include "Client.h"
#include "Skills/Skill.h"

#include <ctime>

Skill::Skill(const Client* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: owner(owner), level(level), type(type), MAX_LEVEL(MAX_LEVEL)
{

}

bool Skill::cast(DIRECTION dir)
{
	clock_t now = clock();
	
	// ���� ���ð� �˻�
	if (now < next_able_time)
		return false; // ����

	next_able_time = now + cooldown;
	return true; // ����
}
int Skill::get_level() const
{
	return level;
}

int Skill::get_damage() const
{
	return damage;
}

int Skill::get_cooldown() const
{
	return cooldown;
}

void Skill::level_up()
{
	level++;
}