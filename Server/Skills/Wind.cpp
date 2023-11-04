#include <list>

#include "Client.h"
#include "Background.h"
#include "Skills/Wind.h"

extern std::list<Client*> clients;
extern Background background;

const int Wind::DAMAGE[] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
const int Wind::COOLDOWN[] = { 170, 170, 170, 170, 170, 170, 170, 170, 170, 170 };

Wind::Wind(const Client* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Skill(owner, level, type, MAX_LEVEL)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
}

bool Wind::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	const COORD pos = owner->get_pos();

	static const int distance = 6;
	static const int delay = 12;

	static const COORD up_hitting_box[] = {
		COORD{-2, -1}, COORD{-1, -2}, COORD{ 0, -2}, COORD{ 1, -2}, COORD{ 2, -1}, COORD{NULL, NULL}
	};
	static const COORD down_hitting_box[] = {
		COORD{-2,  1}, COORD{-1,  2}, COORD{ 0,  2}, COORD{ 1,  2}, COORD{ 2,  1}, COORD{NULL, NULL}
	};
	static const COORD left_hitting_box[] = {
		COORD{-1, -2}, COORD{-2, -1}, COORD{-2,  0}, COORD{-2,  1}, COORD{-2,  2}, COORD{NULL, NULL}
	};
	static const COORD right_hitting_box[] = {
		COORD{ 1, -2}, COORD{ 2, -1}, COORD{ 2,  0}, COORD{ 2,  1}, COORD{ 2,  2}, COORD{NULL, NULL}
	};

	/* 피격 범위 결정 */

	const COORD* hitting_box;
	switch (dir)
	{
	case UP:    hitting_box = &up_hitting_box[0];    break;
	case DOWN:  hitting_box = &down_hitting_box[0];  break;
	case LEFT:  hitting_box = &left_hitting_box[0];  break;
	case RIGHT: hitting_box = &right_hitting_box[0]; break;
	default: return false;
	}

	/* 플레이어 피격 판정 */

	// 검기가 1칸씩 이동(i++)
	for (int i = 0; i < distance; i++)
	{
		// 플레이어를 하나씩 선택
		for (std::list<Client*>::iterator iter = clients.begin();
			iter != clients.end(); iter++)
		{
			Client* client = (*iter);
			COORD client_pos = client->get_pos();

			// hitting_box에 나온 피격지점들을 하나씩 선택하며 플레이어의 위치좌표값을 비교
			for (int j = 0; hitting_box[j].X | hitting_box[j].Y; j++)
			{
				COORD hitting_pos = pos;
				hitting_pos.X += hitting_box[j].X; hitting_pos.Y += hitting_box[j].Y;
				switch (dir)
				{
				case UP:    hitting_pos.Y -= i; break;
				case DOWN:  hitting_pos.Y += i; break;
				case LEFT:  hitting_pos.X -= i; break;
				case RIGHT: hitting_pos.X += i; break;
				}

				if (client_pos.X == hitting_pos.X &&
					client_pos.Y == hitting_pos.Y)
					client->hit(this);
			}
		}

		Sleep(delay);
	}

	return true;
}
void Wind::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
}