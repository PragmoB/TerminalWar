#include "Shoes.h"

Shoes::Shoes(Client* owner, int level) : PassiveSkill(owner, level)
{

}
int Shoes::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}

SKILL_TYPE Shoes::get_type() const
{
	return SHOES;
}
int Shoes::get_max_level() const
{
	return MAX_LEVEL;
}
int Shoes::get_ordinal() const
{
	return 1;
}

bool Shoes::upgradable() const
{
	return false;
}
bool Shoes::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool Shoes::downgradable() const
{
	return false;
}
bool Shoes::downgradable_to(SKILL_TYPE type) const
{
	return false;
}