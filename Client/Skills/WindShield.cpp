#include <iostream>

#include "WindShield.h"

WindShield::WindShield(Player* owner, int level) : WindStep(owner, level)
{

}
int WindShield::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}
int WindShield::get_defense_rate()
{
	return DEFENSE_RATE[get_level() - 1];
}
int WindShield::get_evasion_rate()
{
	return EVASION_RATE[get_level() - 1];
}

SKILL_TYPE WindShield::get_type() const
{
	return WIND_SHIELD;
}
int WindShield::get_max_level() const
{
	return MAX_LEVEL;
}
int WindShield::get_ordinal() const
{
	return 2;
}

bool WindShield::upgradable() const
{
	return false;
}
bool WindShield::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool WindShield::downgradable() const
{
	return true;
}
bool WindShield::downgradable_to(SKILL_TYPE type) const
{
	if (type == WIND_STEP)
		return true;

	return false;
}

const char* WindShield::get_skill_name() const
{
	return "윈드 쉴드";
}
void WindShield::get_level_up_message(char* output, int len) const
{
	const char* skill_name = WindShield::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[패시브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[패시브] %-16s : 이동속도 %d%% 증가 | 방어율 %d%% 증가 | 회피율 %d%% 증가",
			skill_name,
			100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100,
			100 * (DEFENSE_RATE[LEVEL] + 100) / (DEFENSE_RATE[LEVEL - 1] + 100) - 100,
			100 * (EVASION_RATE[LEVEL] + 100) / (EVASION_RATE[LEVEL - 1] + 100) - 100);
	}
}
void WindShield::get_learn_message(char* output, int len) const
{
	const char* skill_name = WindShield::get_skill_name();

	sprintf_s(output, len, "[패시브] %-16s : 바람의 힘이 강화되어 전신을 보호합니다. | 이동속도 %d%% 증가 | 방어율 %d%% 증가 | 회피율 %d%% 증가",
		skill_name,
		SPEED_RATE[0],
		DEFENSE_RATE[0],
		EVASION_RATE[0]);
}