#include "WindStep.h"

WindStep::WindStep(Player* owner, int level) : PassiveSkill(owner, level)
{

}
int WindStep::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}
SKILL_TYPE WindStep::get_type() const
{
	return WIND_STEP;
}
int WindStep::get_max_level() const
{
	return MAX_LEVEL;
}
int WindStep::get_ordinal() const
{
	return 1;
}

bool WindStep::upgradable() const
{
	return true;
}
bool WindStep::upgradable_to(SKILL_TYPE type) const
{
	switch (type)
	{
	case CELESTIAL_ARROW_STEP:
	case HERMES_STEP:
	case WIND_SHIELD:
		return true;
	default:
		return false;
	}
}
bool WindStep::downgradable() const
{
	return false;
}
bool WindStep::downgradable_to(SKILL_TYPE type) const
{
	return false;
}