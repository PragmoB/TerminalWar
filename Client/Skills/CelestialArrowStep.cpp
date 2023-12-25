#include <iostream>

#include "CelestialArrowStep.h"

CelestialArrowStep::CelestialArrowStep(Player* owner, int level) : WindStep(owner, level)
{

}
int CelestialArrowStep::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}
int CelestialArrowStep::get_mov_attack_damage()
{
	return MOV_ATTACK_DAMAGE[get_level() - 1];
}
int CelestialArrowStep::get_mov_attack_range()
{
	return MOV_ATTACK_RANGE;
}

SKILL_TYPE CelestialArrowStep::get_type() const
{
	return CELESTIAL_ARROW_STEP;
}
int CelestialArrowStep::get_max_level() const
{
	return MAX_LEVEL;
}
int CelestialArrowStep::get_ordinal() const
{
	return 2;
}

bool CelestialArrowStep::upgradable() const
{
	return false;
}
bool CelestialArrowStep::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool CelestialArrowStep::downgradable() const
{
	return true;
}
bool CelestialArrowStep::downgradable_to(SKILL_TYPE type) const
{
	if (type == WIND_STEP)
		return true;

	return false;
}

const char* CelestialArrowStep::get_skill_name() const
{
	return "õ�� ȭ�� �亸";
}
void CelestialArrowStep::get_level_up_message(char* output, int len) const
{
	const char* skill_name = CelestialArrowStep::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[�нú�] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[�нú�] %-16s : �̵��ӵ� %d%% ���� | ������ %d%% ����",
			skill_name, 100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100,
			100 * (MOV_ATTACK_DAMAGE[LEVEL] - MOV_ATTACK_DAMAGE[LEVEL - 1]) / MOV_ATTACK_DAMAGE[LEVEL - 1]);
	}
}
void CelestialArrowStep::get_learn_message(char* output, int len) const
{
	const char* skill_name = CelestialArrowStep::get_skill_name();

	sprintf_s(output, len, "[�нú�] %-16s : �߰����� �ٶ��� ������ ȭ��ó�� ��ī�ο� ���� �ο��մϴ�. | �̵��ӵ� %d%% ���� | ������ %d%% | ���� ���� %dĭ",
		skill_name,
		SPEED_RATE[0],
		MOV_ATTACK_DAMAGE[0],
		MOV_ATTACK_RANGE);
}