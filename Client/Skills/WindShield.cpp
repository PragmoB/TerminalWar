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
	return "���� ����";
}
void WindShield::get_level_up_message(char* output, int len) const
{
	const char* skill_name = WindShield::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[�нú�] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[�нú�] %-16s : �̵��ӵ� %d%% ���� | ����� %d%% ���� | ȸ���� %d%% ����",
			skill_name,
			100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100,
			100 * (DEFENSE_RATE[LEVEL] + 100) / (DEFENSE_RATE[LEVEL - 1] + 100) - 100,
			100 * (EVASION_RATE[LEVEL] + 100) / (EVASION_RATE[LEVEL - 1] + 100) - 100);
	}
}
void WindShield::get_learn_message(char* output, int len) const
{
	const char* skill_name = WindShield::get_skill_name();

	sprintf_s(output, len, "[�нú�] %-16s : �ٶ��� ���� ��ȭ�Ǿ� ������ ��ȣ�մϴ�. | �̵��ӵ� %d%% ���� | ����� %d%% ���� | ȸ���� %d%% ����",
		skill_name,
		SPEED_RATE[0],
		DEFENSE_RATE[0],
		EVASION_RATE[0]);
}