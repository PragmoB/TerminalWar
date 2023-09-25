#include <list>

#include "Client.h"
#include "Bullet.h"

extern std::list<Client*> clients;

// pos ��ġ�� Ŭ���̾�Ʈ�� �ϳ��� ã�ƿ���
Client* get_client_at_pos(POINT pos)
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		Client* cur = *iter;
		POINT cur_pos = cur->getPos();

		if (cur_pos.x == pos.x && cur_pos.y == pos.y)
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
		for (; 0 < distance && 1 < y; distance--)
		{
			y--; // �Ѿ��� ���� �̵�
			Client* victim = get_client_at_pos(POINT{ x, y });
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps); // �߻� �ӵ� ����
		}
		break;
	case DOWN:
		for (; 0 < distance && y < field_height; distance--)
		{
			y++;
			Client* victim = get_client_at_pos(POINT{ x, y });
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case LEFT:
		for (; 0 < distance && 1 < x; distance--)
		{
			x--;
			Client* victim = get_client_at_pos(POINT{ x, y });
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	case RIGHT:
		for (; 0 < distance && x < field_width; distance--)
		{
			x++;
			Client* victim = get_client_at_pos(POINT{ x, y });
			if (victim) // ���� ���� �ִٸ�
				victim->hit(); // ó�´� ������

			Sleep(1000 / bps);
		}
		break;
	}
}