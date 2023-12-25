#include <iostream>

#include "WindStep.h"

WindStep::WindStep(Player* owner, int level) : PassiveSkill(owner, level)
{

}
int WindStep::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}

SKILL_TYPE WindStep::get_type() const
{
	return WIND_STEP;
}
int WindStep::get_max_level() const
{
	return MAX_LEVEL;
}
int WindStep::get_ordinal() const
{
	return 1;
}

bool WindStep::upgradable() const
{
	return true;
}
bool WindStep::upgradable_to(SKILL_TYPE type) const
{
	switch (type)
	{
	case CELESTIAL_ARROW_STEP:
	case HERMES_STEP:
	case WIND_SHIELD:
		return true;
	default:
		return false;
	}
}
bool WindStep::downgradable() const
{
	return false;
}
bool WindStep::downgradable_to(SKILL_TYPE type) const
{
	return false;
}

const char* WindStep::get_skill_name() const
{
	return "윈드 스텝";
}
void WindStep::get_level_up_message(char* output, int len) const
{
	const char* skill_name = WindStep::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[패시브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[패시브] %-16s : 이동속도 %d%% 증가",
			skill_name, 100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100);
	}
}
void WindStep::get_learn_message(char* output, int len) const
{
	const char* skill_name = WindStep::get_skill_name();

	sprintf_s(output, len, "[패시브] %-16s : 발걸음에 바람의 힘이 깃들어 신속하게 이동합니다. | 이동속도 %d%% 증가",
		skill_name,
		SPEED_RATE[0]);
}