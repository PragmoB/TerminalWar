#include <cstring>

#include "Skills/Snipe.h"
#include "Player.h"
#include "Graphic.h"
#include "interface.h"

extern Graphic graphic;

Snipe::Snipe(Player* owner, int level)
	: Shoot(owner, level)
{
}

/* 총알 위치를 조정하고, graphic에 반영함 */

bool Snipe::cast(DIRECTION dir)
{
	return Shoot::cast(dir);
}

int Snipe::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int Snipe::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
int Snipe::get_bps() const
{
	return BPS[get_level() - 1];
}
int Snipe::get_distance() const
{
	return DISTANCE[get_level() - 1];
}
SKILL_TYPE Snipe::get_type() const
{
	return SNIPE;
}
int Snipe::get_max_level() const
{
	return MAX_LEVEL;
}
const char* Snipe::get_skill_name() const
{
	return "저격";
}
void Snipe::get_level_up_message(char* output, int len) const
{
	const char* skill_name = Snipe::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[액티브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[액티브] %-16s : 공격력 %d%% 증가 | 쿨타임 %d%% 감소 | 탄속 %d%% 증가 | 사거리 %d칸 증가",
			skill_name,
			100 * (DAMAGE[LEVEL] - DAMAGE[LEVEL - 1]) / DAMAGE[LEVEL - 1],
			100 * (COOLDOWN[LEVEL - 1] - COOLDOWN[LEVEL]) / COOLDOWN[LEVEL - 1],
			100 * (BPS[LEVEL] - BPS[LEVEL - 1]) / BPS[LEVEL - 1],
			DISTANCE[LEVEL] - DISTANCE[LEVEL - 1]);
	}
}
void Snipe::get_learn_message(char* output, int len) const
{
	const char* skill_name = Snipe::get_skill_name();

	sprintf_s(output, len, "[액티브] %-16s : 강력한 데미지와 높은 정확성으로 적을 향해 총을 쏩니다. | 공격력 %d | 쿨타임 %d.%d초 | 탄속 %dbps | 사거리 %d칸",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000,
		BPS[0],
		DISTANCE[0]);
}
int Snipe::get_ordinal() const
{
	return 2;
}
bool Snipe::upgradable() const
{
	return false;
}
bool Snipe::upgradable_to(SKILL_TYPE type) const
{
	return false;
}