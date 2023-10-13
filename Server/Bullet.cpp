#include <list>

#include "Client.h"
#include "Bullet.h"

extern std::list<Client*> clients;

// pos ��ġ�� Ŭ���̾�Ʈ�� �ϳ��� ã�ƿ���
Client* get_client_at_pos(COORD pos)
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		Client* cur = *iter;
		COORD cur_pos = cur->getPos();

		if (cur_pos.X == pos.X && cur_pos.Y == pos.Y)
			return cur;
	}
	return NULL;
}
// Ŭ���̾�Ʈ �ǰ�����
void Bullet::fire()
{
	switch (dir)
	{
	case UP:
		// ���� ��ų� �����Ÿ��� 0�� �� ������ ��� ���ư�
		for (; 0 < distance && 1 < pos.Y; distance--)
		{
			pos.Y--; // �Ѿ��� ���� �̵�
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps); // �߻� �ӵ� ����
		}
		break;
	case DOWN:
		for (; 0 < distance && pos.Y < field_height; distance--)
		{
			pos.Y++;
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case LEFT:
		for (; 0 < distance && 1 < pos.X; distance--)
		{
			pos.X--;
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case RIGHT:
		for (; 0 < distance && pos.X < field_width; distance--)
		{
			pos.X++;
			Client* victim = get_client_at_pos(pos);
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	}
}