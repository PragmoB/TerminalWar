#include "HermesStep.h"

HermesStep::HermesStep(Player* owner, int level) : WindStep(owner, level)
{

}
int HermesStep::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}
SKILL_TYPE HermesStep::get_type() const
{
	return HERMES_STEP;
}
int HermesStep::get_max_level() const
{
	return MAX_LEVEL;
}
int HermesStep::get_ordinal() const
{
	return 2;
}

bool HermesStep::upgradable() const
{
	return false;
}
bool HermesStep::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool HermesStep::downgradable() const
{
	return true;
}
bool HermesStep::downgradable_to(SKILL_TYPE type) const
{
	if (type == WIND_STEP)
		return true;

	return WindStep::downgradable_to(type);
}