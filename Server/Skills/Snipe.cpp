#include <list>

#include "Client.h"
#include "Background.h"
#include "Skills/Skill.h"
#include "Skills/Snipe.h"

extern Background background;

Snipe::Snipe(Client* owner, int level)
	: Shoot(owner, level)
{

}

// 클라이언트 피격판정
bool Snipe::cast(DIRECTION dir)
{
	return Shoot::cast(dir);
}

int Snipe::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int Snipe::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
int Snipe::get_bps() const
{
	return BPS[get_level() - 1];
}
int Snipe::get_distance() const
{
	return DISTANCE[get_level() - 1];
}
SKILL_TYPE Snipe::get_type() const
{
	return SNIPE;
}
int Snipe::get_max_level() const
{
	return MAX_LEVEL;
}
int Snipe::get_ordinal() const
{
	return 2;
}
bool Snipe::upgradable() const
{
	return false;
}
bool Snipe::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool Snipe::downgradable() const
{
	return true;
}

bool Snipe::downgradable_to(SKILL_TYPE type) const
{
	if (type == SHOOT)
		return true;

	return Shoot::downgradable_to(type);
}