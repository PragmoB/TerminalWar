#include "protocol.h"
#include "Player.h"
#include "Sound.h"
#include <ctime>

#include <map>

#include "Skills/Skill.h"
#include "Skills/Shoot.h"
#include "Skills/Snipe.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/Wind.h"
#include "Skills/WindSlash.h"
#include "Skills/LightningStrike.h"
#include "Skills/WindStep.h"
#include "Skills/CelestialArrowStep.h"
#include "Skills/HermesStep.h"
#include "Skills/WindShield.h"

extern Sound sound;

Skill::Skill(Player* owner, int level)
	: owner(owner), level(level)
{

}

Skill* Skill::create_object_by_type(SKILL_TYPE type, Player* owner, int level)
{
	Skill* skill = NULL;

	switch (type)
	{
	case SHOOT:				   skill = new Shoot(owner, level);				 break;
	case SNIPE:				   skill = new Snipe(owner, level);				 break;
	case SLASH:				   skill = new Slash(owner, level);				 break;
	case LIGHTSABER_SLASH:     skill = new LightsaberSlash(owner, level);	 break;
	case ZWEIHANDER_SLASH:	   skill = new ZweihanderSlash(owner, level);	 break;
	case WIND:				   skill = new Wind(owner, level);				 break;
	case WIND_SLASH:		   skill = new WindSlash(owner, level);			 break;
	case LIGHTNING_STRIKE:	   skill = new LightningStrike(owner, level);	 break;
	case WIND_STEP:			   skill = new WindStep(owner, level);			 break;
	case CELESTIAL_ARROW_STEP: skill = new CelestialArrowStep(owner, level); break;
	case HERMES_STEP:		   skill = new HermesStep(owner, level);		 break;
	case WIND_SHIELD:		   skill = new WindShield(owner, level);		 break;
	}

	return skill;

}
const Skill* Skill::get_object_by_type(SKILL_TYPE type)
{
	static std::map<SKILL_TYPE, Skill*> data;

	Skill* skill = data[type];
	if (skill)
		return skill;
	else
	{
		data[type] = skill = create_object_by_type(type, NULL);
		return skill;
	}
}
void Skill::level_up()
{
	if (level < get_max_level())
		level++;
}
int Skill::get_level() const
{
	return level;
}
Player* Skill::get_owner() const
{
	return owner;
}