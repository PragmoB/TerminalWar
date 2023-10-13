#include <list>

#include "Client.h"
#include "Bullet.h"

extern std::list<Client*> clients;

// pos 위치의 클라이언트를 하나만 찾아오기
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
// 클라이언트 피격판정
void Bullet::fire()
{
	switch (dir)
	{
	case UP:
		// 벽에 닿거나 사정거리가 0이 될 때까지 계속 날아감
		for (; 0 < distance && 1 < pos.Y; distance--)
		{
			pos.Y--; // 총알이 위로 이동
			Client* victim = get_client_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(); // 처맞는 동작함

			Sleep(1000 / bps); // 발사 속도 조절
		}
		break;
	case DOWN:
		for (; 0 < distance && pos.Y < field_height; distance--)
		{
			pos.Y++;
			Client* victim = get_client_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(); // 처맞는 동작함

			Sleep(1000 / bps);
		}
		break;
	case LEFT:
		for (; 0 < distance && 1 < pos.X; distance--)
		{
			pos.X--;
			Client* victim = get_client_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(); // 처맞는 동작함

			Sleep(1000 / bps);
		}
		break;
	case RIGHT:
		for (; 0 < distance && pos.X < field_width; distance--)
		{
			pos.X++;
			Client* victim = get_client_at_pos(pos);
			if (victim) // 맞은 놈이 있다면
				victim->hit(); // 처맞는 동작함

			Sleep(1000 / bps);
		}
		break;
	}
}