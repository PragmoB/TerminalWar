#include <list>

#include "Client.h"
#include "Background.h"
#include "Skills/WindSlash.h"

extern std::list<Client*> clients;
extern Background background;

const int WindSlash::DAMAGE[] = { 120, 126, 132, 138, 144, 151, 158, 165, 173, 181 };
const int WindSlash::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };

WindSlash::WindSlash(Client* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Slash(owner, level, type, MAX_LEVEL), wind(owner)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
}

bool WindSlash::cast(DIRECTION dir)
{
	background.cast_skill(&wind, dir);
	Slash::cast(dir);

	return true;
}
void WindSlash::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
}