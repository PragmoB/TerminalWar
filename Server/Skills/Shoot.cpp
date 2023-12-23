#include <list>

#include "Client.h"
#include "Background.h"
#include "Skills/Shoot.h"

extern Background background;

Shoot::Shoot(Client* owner, int level)
	: ActiveSkill(owner, level)
{

}

// pos ��ġ�� Ŭ���̾�Ʈ�� �ϳ��� ã�ƿ���
Client* get_client_at_pos(COORD pos)
{
	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
	{
		Client* cur = *iter;
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

	Client* owner = get_owner();
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
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

			Sleep(1000 / bps); // �߻� �ӵ� ����
		}
		break;
	case DOWN:
		for (; 0 < remain_distance && pos.Y < FIELD_HEIGHT; remain_distance--)
		{
			pos.Y++;
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case LEFT:
		for (; 0 < remain_distance && 1 < pos.X; remain_distance--)
		{
			pos.X--;
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(this); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case RIGHT:
		for (; 0 < remain_distance && pos.X < FIELD_WIDTH; remain_distance--)
		{
			pos.X++;
			Client* victim = get_client_at_pos(pos);
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