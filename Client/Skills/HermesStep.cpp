#include <iostream>

#include "HermesStep.h"

HermesStep::HermesStep(Player* owner, int level) : WindStep(owner, level)
{

}
int HermesStep::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}

SKILL_TYPE HermesStep::get_type() const
{
	return HERMES_STEP;
}
int HermesStep::get_max_level() const
{
	return MAX_LEVEL;
}
int HermesStep::get_ordinal() const
{
	return 2;
}

bool HermesStep::upgradable() const
{
	return false;
}
bool HermesStep::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool HermesStep::downgradable() const
{
	return true;
}
bool HermesStep::downgradable_to(SKILL_TYPE type) const
{
	if (type == WIND_STEP)
		return true;

	return false;
}

const char* HermesStep::get_skill_name() const
{
	return "헤르메스 스텝";
}
void HermesStep::get_level_up_message(char* output, int len) const
{
	const char* skill_name = HermesStep::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[패시브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[패시브] %-16s : 이동속도 %d%% 증가",
			skill_name, 100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100);
	}
}
void HermesStep::get_learn_message(char* output, int len) const
{
	const char* skill_name = HermesStep::get_skill_name();

	sprintf_s(output, len, "[패시브] %-16s : 헤르메스 신의 축복을 받아 발걸음이 매우 가벼워집니다. | 이동속도 %d%% 증가",
		skill_name,
		SPEED_RATE[0]);
}