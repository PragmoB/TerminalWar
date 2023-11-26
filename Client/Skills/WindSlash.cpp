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
	return "ǳ����(��ب��)";
}
void WindSlash::get_level_up_message(char* output, int len) const
{
	const char* skill_name = WindSlash::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[��Ƽ��] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[��Ƽ��] %-16s : ���ݷ� %d%% ���� | ��Ÿ�� %d%% ���� | �˱� ���ݷ� %d%% ���� | �˱� �ӵ� %d%% ���� | �˱� ��Ÿ� %dĭ ����",
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

	sprintf_s(output, len, "[��Ƽ��] %-16s : ���� �ֵθ��� �˱⸦ �����ϴ�. | ���ݷ� %d | ��Ÿ�� %d.%d�� | �˱� ���ݷ� %d | �˱� �ӵ� %d | �˱� ��Ÿ� %dĭ",
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