#include <list>

#include "Client.h"
#include "Background.h"
#include "Skills/Wind.h"

extern Background background;

Wind::Wind(Client* owner, int level)
	: Skill(owner, level)
{
}

bool Wind::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Client* owner = get_owner();
	const COORD pos = owner->get_pos();

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

	const int bps = get_bps();
	const int distance = get_distance();

	// 검기가 1칸씩 이동(i++)
	for (int i = 0; i < distance; i++)
	{
		// 플레이어를 하나씩 선택
		for (std::list<Client*>::iterator iter = background.clients.begin();
			iter != background.clients.end(); iter++)
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

		Sleep(1000 / bps);
	}

	return true;
}

int Wind::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int Wind::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
int Wind::get_bps() const
{
	return BPS[get_level() - 1];
}
int Wind::get_distance() const
{
	return DISTANCE[get_level() - 1];
}
SKILL_TYPE Wind::get_type() const
{
	return WIND;
}
int Wind::get_max_level() const
{
	return MAX_LEVEL;
}
int Wind::get_ordinal() const
{
	return 2;
}
bool Wind::upgradable() const
{
	return false;
}
bool Wind::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool Wind::downgradable() const
{
	return false;
}

bool Wind::downgradable_to(SKILL_TYPE type) const
{
	return false;
}