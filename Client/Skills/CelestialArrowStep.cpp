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
	return "천공 화살 답보";
}
void CelestialArrowStep::get_level_up_message(char* output, int len) const
{
	const char* skill_name = CelestialArrowStep::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[패시브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[패시브] %-16s : 이동속도 %d%% 증가 | 데미지 %d%% 증가",
			skill_name, 100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100,
			100 * (MOV_ATTACK_DAMAGE[LEVEL] - MOV_ATTACK_DAMAGE[LEVEL - 1]) / MOV_ATTACK_DAMAGE[LEVEL - 1]);
	}
}
void CelestialArrowStep::get_learn_message(char* output, int len) const
{
	const char* skill_name = CelestialArrowStep::get_skill_name();

	sprintf_s(output, len, "[패시브] %-16s : 발걸음에 바람을 가르는 화살처럼 날카로운 힘을 부여합니다. | 이동속도 %d%% 증가 | 데미지 %d%% | 공격 범위 %d칸",
		skill_name,
		SPEED_RATE[0],
		MOV_ATTACK_DAMAGE[0],
		MOV_ATTACK_RANGE);
}