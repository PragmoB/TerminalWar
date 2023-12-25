#include "WindShield.h"

WindShield::WindShield(Player* owner, int level) : WindStep(owner, level)
{

}
int WindShield::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}
int WindShield::get_defense_rate()
{
	return DEFENSE_RATE[get_level() - 1];
}
int WindShield::get_evasion_rate()
{
	return EVASION_RATE[get_level() - 1];
}
SKILL_TYPE WindShield::get_type() const
{
	return WIND_SHIELD;
}
int WindShield::get_max_level() const
{
	return MAX_LEVEL;
}
int WindShield::get_ordinal() const
{
	return 2;
}

bool WindShield::upgradable() const
{
	return false;
}
bool WindShield::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool WindShield::downgradable() const
{
	return true;
}
bool WindShield::downgradable_to(SKILL_TYPE type) const
{
	if (type == WIND_STEP)
		return true;

	return WindStep::downgradable_to(type);
}