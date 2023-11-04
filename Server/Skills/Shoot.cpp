#include <list>

#include "Client.h"
#include "Skills/Shoot.h"

extern std::list<Client*> clients;

const int Shoot::DAMAGE[] = { 10, 10, 10, 10, 10 };
const int Shoot::COOLDOWN[] = { 170, 170, 170, 170, 170 };
const int Shoot::BPS[] = { 25, 25, 25, 25, 25 };
const int Shoot::DISTANCE[] = { 10, 10, 10, 10, 10 };

Shoot::Shoot(const Client* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Skill(owner, level, type, MAX_LEVEL)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
	bps = BPS[level - 1];
	distance = DISTANCE[level - 1];
}

// pos ��ġ�� Ŭ���̾�Ʈ�� �ϳ��� ã�ƿ���
Client* get_client_at_pos(COORD pos)
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
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
	if (!Skill::cast(dir))
		return false;

	COORD pos = owner->get_pos();

	int remain_distance = distance;

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
void Shoot::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
	bps = BPS[get_level() - 1];
	distance = DISTANCE[get_level() - 1];
}