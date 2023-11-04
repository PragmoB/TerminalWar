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
	
	// 재사용 대기시간 검사
	if (now < next_able_time)
		return false; // 실패

	next_able_time = now + cooldown;
	return true; // 성공
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