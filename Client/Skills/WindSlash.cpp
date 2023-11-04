#include "Graphic.h"
#include "Player.h"

#include "Skills/WindSlash.h"

extern Graphic graphic;

const int WindSlash::DAMAGE[] = { 120, 126, 132, 138, 144, 151, 158, 165, 173, 181 };
const int WindSlash::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };

WindSlash::WindSlash(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Slash(owner, level, type, MAX_LEVEL), wind(owner)
{	
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
}

bool WindSlash::cast(DIRECTION dir)
{
	graphic.cast_skill(&wind, dir);
	Slash::cast(dir);

	return true;
}
void WindSlash::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();
	wind.level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
}