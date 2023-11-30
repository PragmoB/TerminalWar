#include "Client.h"

#include "Skills/Skill.h"
#include "Skills/Shoot.h"
#include "Skills/Snipe.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/Wind.h"
#include "Skills/WindSlash.h"
#include "Skills/LightningStrike.h"

#include <ctime>

#include <map>

Skill::Skill(Client* owner, int level)
	: owner(owner), level(level)
{

}

Skill* Skill::create_object_by_type(SKILL_TYPE type, Client* owner)
{
	Skill* skill = NULL;

	switch (type)
	{
	case SHOOT:			   skill = new Shoot(owner);		   break;
	case SNIPE:			   skill = new Snipe(owner);		   break;
	case SLASH:			   skill = new Slash(owner);		   break;
	case LIGHTSABER_SLASH: skill = new LightsaberSlash(owner); break;
	case ZWEIHANDER_SLASH: skill = new ZweihanderSlash(owner); break;
	case WIND:			   skill = new Wind(owner);			   break;
	case WIND_SLASH:	   skill = new WindSlash(owner);	   break;
	case LIGHTNING_STRIKE: skill = new LightningStrike(owner); break;
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
bool Skill::castable() const
{
	clock_t now = clock();

	if (now < next_able_time)
		return false;

	return true;
}
bool Skill::cast(DIRECTION dir)
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
int Skill::get_level() const
{
	return level;
}

Client* Skill::get_owner() const
{
	return owner;
}

bool Skill::level_up()
{
	if (level < get_max_level())
	{
		level++;
		return true;
	}
	else
		return false;
}