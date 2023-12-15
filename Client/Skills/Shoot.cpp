#include <cstring>

#include "Skills/Shoot.h"
#include "Player.h"
#include "Graphic.h"
#include "interface.h"

extern Graphic graphic;

Shoot::Shoot(Player* owner, int level)
	: Skill(owner, level)
{
}

/* 총알 위치를 조정하고, graphic에 반영함 */

bool Shoot::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Player* owner = get_owner();
	COORD pos = owner->get_pos();

	const int bps = get_bps();
	int remain_distance = get_distance();
	switch (dir)
	{
	case UP:
		// 벽에 닿거나 사정거리가 0이 될 때까지 계속 날아감
		for (; 0 < remain_distance && FIELD.Top < pos.Y - 1; remain_distance--)
		{
			pos.Y--; // 총알이 위로 이동
			graphic.draw(pos, '|'); // 그래픽에 반영

			Sleep(1000 / bps); // 발사 속도 조절
			graphic.draw(pos, ' '); // 잔상 지우기
		}
		break;
	case DOWN:
		for (; 0 < remain_distance && pos.Y < FIELD.Bottom; remain_distance--)
		{
			pos.Y++;
			graphic.draw(pos, '|'); // 그래픽에 반영

			Sleep(1000 / bps);
			graphic.draw(pos, ' '); // 잔상 지우기
		}
		break;
	case LEFT:
		for (; 0 < remain_distance && FIELD.Left < pos.X - 2; remain_distance--)
		{
			pos.X -= 2;
			graphic.draw(pos, "--"); // 그래픽에 반영

			Sleep(1000 / bps);
			graphic.draw(pos, "  "); // 잔상 지우기
		}
		break;
	case RIGHT:
		pos.X--;
		for (; 0 < remain_distance && pos.X + 2 < FIELD.Left + 2 * FIELD_WIDTH; remain_distance--)
		{
			pos.X += 2;
			graphic.draw(pos, "--"); // 그래픽에 반영

			Sleep(1000 / bps);
			graphic.draw(pos, "  "); // 잔상 지우기
		}
		break;
	}

	return true;
}

int Shoot::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int Shoot::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
int Shoot::get_bps() const
{
	return BPS[get_level() - 1];
}
int Shoot::get_distance() const
{
	return DISTANCE[get_level() - 1];
}
SKILL_TYPE Shoot::get_type() const
{
	return SHOOT;
}
int Shoot::get_max_level() const
{
	return MAX_LEVEL;
}
const char* Shoot::get_skill_name() const
{
	return "입자방출";
}
void Shoot::get_level_up_message(char* output, int len) const
{
	const char* skill_name = Shoot::get_skill_name();
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
void Shoot::get_learn_message(char* output, int len) const
{
	const char* skill_name = Shoot::get_skill_name();

	sprintf_s(output, len, "[액티브] %-16s : 잠재된 마력을 끌어내 전방으로 입자를 발사합니다. | 공격력 %d | 쿨타임 %d.%d초 | 탄속 %dbps | 사거리 %d칸",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000,
		BPS[0],
		DISTANCE[0]);
}
int Shoot::get_ordinal() const
{
	return 1;
}
bool Shoot::upgradable() const
{
	return true;
}
bool Shoot::upgradable_to(SKILL_TYPE type) const
{
	switch (type)
	{
	case SNIPE:
		return true;
	default:
		return false;
	}
}