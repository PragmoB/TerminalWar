#include <list>

#include "Shoot.h"
#include "Background.h"

#include "Values/interface.h"

extern Background background;

Shoot::Shoot(Player* owner, int level)
	: ActiveSkill(owner, level)
{

}

// pos ��ġ�� Ŭ���̾�Ʈ�� �ϳ��� ã�ƿ���
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
// Ŭ���̾�Ʈ �ǰ�����
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
		// ���� ��ų� �����Ÿ��� 0�� �� ������ ��� ���ư�
		for (; 0 < remain_distance && 1 < pos.Y; remain_distance--)
		{
			pos.Y--; // �Ѿ��� ���� �̵�
			Player* victim = get_player_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

			Sleep(1000 / bps); // �߻� �ӵ� ����
		}
		break;
	case DOWN:
		for (; 0 < remain_distance && pos.Y < FIELD_HEIGHT; remain_distance--)
		{
			pos.Y++;
			Player* victim = get_player_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case LEFT:
		for (; 0 < remain_distance && 1 < pos.X; remain_distance--)
		{
			pos.X--;
			Player* victim = get_player_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case RIGHT:
		for (; 0 < remain_distance && pos.X < FIELD_WIDTH; remain_distance--)
		{
			pos.X++;
			Player* victim = get_player_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

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