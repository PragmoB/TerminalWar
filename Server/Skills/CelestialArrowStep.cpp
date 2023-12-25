#include "CelestialArrowStep.h"

CelestialArrowStep::CelestialArrowStep(Client* owner, int level) : WindStep(owner, level)
{

}
int CelestialArrowStep::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}
int CelestialArrowStep::get_mov_attack_damage()
{
	return MOV_ATTACK_DAMAGE[get_level() - 1];
}
int CelestialArrowStep::get_mov_attack_range()
{
	return MOV_ATTACK_RANGE;
}
SKILL_TYPE CelestialArrowStep::get_type() const
{
	return CELESTIAL_ARROW_STEP;
}
int CelestialArrowStep::get_max_level() const
{
	return MAX_LEVEL;
}
int CelestialArrowStep::get_ordinal() const
{
	return 2;
}

bool CelestialArrowStep::upgradable() const
{
	return false;
}
bool CelestialArrowStep::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool CelestialArrowStep::downgradable() const
{
	return true;
}
bool CelestialArrowStep::downgradable_to(SKILL_TYPE type) const
{
	if (type == WIND_STEP)
		return true;

	return WindStep::downgradable_to(type);
}