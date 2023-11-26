#include "Graphic.h"
#include "Player.h"

#include "Skills/WindSlash.h"

extern Graphic graphic;

WindSlash::WindSlash(Player* owner, int level)
	: Slash(owner, level), wind(owner, level)
{	
}

bool WindSlash::cast(DIRECTION dir)
{
	graphic.cast_skill(&wind, dir);
	Slash::cast(dir);

	return true;
}
void WindSlash::level_up()
{
	Skill::level_up();
	wind.level_up();
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
const char* WindSlash::get_skill_name() const
{
	return "풍마참(風磨斬)";
}
void WindSlash::get_level_up_message(char* output, int len) const
{
	const char* skill_name = WindSlash::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[액티브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[액티브] %-16s : 공격력 %d%% 증가 | 쿨타임 %d%% 감소 | 검기 공격력 %d%% 증가 | 검기 속도 %d%% 증가 | 검기 사거리 %d칸 증가",
			skill_name,
			100 * (DAMAGE[LEVEL + 1] - DAMAGE[LEVEL]) / DAMAGE[LEVEL],
			100 * (COOLDOWN[LEVEL] - COOLDOWN[LEVEL + 1]) / COOLDOWN[LEVEL],
			100 * (Wind::DAMAGE[LEVEL + 1] - Wind::DAMAGE[LEVEL]) / Wind::DAMAGE[LEVEL],
			100 * (Wind::BPS[LEVEL + 1] - Wind::BPS[LEVEL]) / Wind::BPS[LEVEL],
			Wind::DISTANCE[LEVEL + 1] - Wind::DISTANCE[LEVEL]);
	}
}
void WindSlash::get_learn_message(char* output, int len) const
{
	const char* skill_name = WindSlash::get_skill_name();

	sprintf_s(output, len, "[액티브] %-16s : 검을 휘두르며 검기를 날립니다. | 공격력 %d | 쿨타임 %d.%d초 | 검기 공격력 %d | 검기 속도 %d | 검기 사거리 %d칸",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000,
		Wind::DAMAGE[0],
		Wind::BPS[0],
		Wind::DISTANCE[0]);
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