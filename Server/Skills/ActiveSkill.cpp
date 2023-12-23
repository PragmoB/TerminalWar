#include "ActiveSkill.h"

ActiveSkill::ActiveSkill(Client* owner, int level) : Skill(owner, level)
{
	
}
bool ActiveSkill::castable() const
{
	clock_t now = clock();

	if (now < next_able_time)
		return false;

	return true;
}
bool ActiveSkill::cast(DIRECTION dir)
{
	clock_t now = clock();

	if (castable())
	{
		next_able_time = now + get_cooldown();
		return true;
	}
	else
		return false;
}