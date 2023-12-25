#include "Background.h"
#include "WindSlash.h"

extern Background background;

WindSlash::WindSlash(Player* owner, int level)
	: Slash(owner, level), wind(owner, level)
{
}

bool WindSlash::cast(DIRECTION dir)
{
	background.cast_skill(&wind, dir);
	Slash::cast(dir);

	return true;
}
bool WindSlash::level_up()
{
	return (wind.level_up() && Skill::level_up());
}
int WindSlash::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int WindSlash::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
SKILL_TYPE WindSlash::get_type() const
{
	return WIND_SLASH;
}
int WindSlash::get_max_level() const
{
	return MAX_LEVEL;
}
int WindSlash::get_ordinal() const
{
	return 2;
}
bool WindSlash::upgradable() const
{
	return false;
}
bool WindSlash::upgradable_to(SKILL_TYPE type) const
{
	return false;
}

bool WindSlash::downgradable() const
{
	return true;
}

bool WindSlash::downgradable_to(SKILL_TYPE type) const
{
	if (type == SLASH)
		return true;

	return Slash::downgradable_to(type);
}