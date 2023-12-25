#include <list>

#include "Shoot.h"
#include "Background.h"

#include "Values/interface.h"

extern Background background;

Shoot::Shoot(Player* owner, int level)
	: ActiveSkill(owner, level)
{

}

// pos 위치의 클라이언트를 하나만 찾아오기
Player* get_player_at_pos(COORD pos)
{
	for (std::list<Player*>::iterator iter = background.players.begin();
		iter != background.players.end(); iter++)
	{
		Player* cur = *iter;
		COORD cur_pos = cur->get_pos();

		if (cur_pos.X == pos.X && cur_pos.Y == pos.Y)
			return cur;
	}
	return NULL;
}
// 클라이언트 피격판정
bool Shoot::cast(DIRECTION dir)
{
	if (!ActiveSkill::cast(dir))
		return false;

	Player* owner = get_owner();
	COORD pos = owner->get_pos();
	owner->bind(40);

	const int bps = get_bps();
	int remain_distance = get_distance();

	switch (dir)
	{
	case UP:
		// 벽에 닿거나 사정거리가 0이 될 때까지 계속 날아감
		for (; 0 < remain_distance && 1 < pos.Y; remain_distance--)
		{
			pos.Y--; // 총알이 위로 이동
			Player* victim = get_player_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(this); // 처맞는 동작함

			Sleep(1000 / bps); // 발사 속도 조절
		}
		break;
	case DOWN:
		for (; 0 < remain_distance && pos.Y < FIELD_HEIGHT; remain_distance--)
		{
			pos.Y++;
			Player* victim = get_player_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(this); // 처맞는 동작함

			Sleep(1000 / bps);
		}
		break;
	case LEFT:
		for (; 0 < remain_distance && 1 < pos.X; remain_distance--)
		{
			pos.X--;
			Player* victim = get_player_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(this); // 처맞는 동작함

			Sleep(1000 / bps);
		}
		break;
	case RIGHT:
		for (; 0 < remain_distance && pos.X < FIELD_WIDTH; remain_distance--)
		{
			pos.X++;
			Player* victim = get_player_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(this); // 처맞는 동작함

			Sleep(1000 / bps);
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
bool Shoot::downgradable() const
{
	return false;
}

bool Shoot::downgradable_to(SKILL_TYPE type) const
{
	return false;
}