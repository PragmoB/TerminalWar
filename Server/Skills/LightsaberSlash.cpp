#include <list>

#include "Client.h"
#include "background.h"
#include "Skills/LightsaberSlash.h"

extern Background background;

LightsaberSlash::LightsaberSlash(Client* owner, int level)
	: Slash(owner, level)
{

}

bool LightsaberSlash::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Client* owner = get_owner();
	const COORD pos = owner->get_pos();
	static const int delay[] = { 50, 20, 20, 10, 10, 10, 0, NULL };
	owner->bind(120);

	/* pos�� �������� �ǰ� ����(hitting box) ���� */

	static const COORD up_hitting_box[][5] = {
		{ COORD{-1,  1}, COORD{NULL, NULL} },
		{ COORD{-2,  1}, COORD{-2,  0}, COORD{-1,  0}, COORD{NULL, NULL} },
		{ COORD{-2, -1}, COORD{-1, -1}, COORD{-1, -2}, COORD{NULL, NULL} },
		{ COORD{ 0, -2}, COORD{ 0, -1}, COORD{NULL, NULL} },
		{ COORD{ 1, -2}, COORD{NULL, NULL} },
		{ COORD{ 1, -1}, COORD{ 2, -1}, COORD{ 1,  0}, COORD{ 2,  0}, COORD{NULL, NULL} },
		{ COORD{ 1,  1}, COORD{ 2,  1}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};
	static const COORD down_hitting_box[][5] = {
		{ COORD{-1, -1}, COORD{NULL, NULL} },
		{ COORD{-2, -1}, COORD{-2,  0}, COORD{-1,  0}, COORD{NULL, NULL} },
		{ COORD{-2,  1}, COORD{-1,  1}, COORD{-1,  2}, COORD{NULL, NULL} },
		{ COORD{ 0,  2}, COORD{ 0,  1}, COORD{NULL, NULL} },
		{ COORD{ 1,  2}, COORD{NULL, NULL} },
		{ COORD{ 1,  1}, COORD{ 2,  1}, COORD{ 1,  0}, COORD{ 2,  0}, COORD{NULL, NULL} },
		{ COORD{ 1, -1}, COORD{ 2, -1}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};
	static const COORD left_hitting_box[][5] = {
		{ COORD{ 1, -2}, COORD{NULL, NULL} },
		{ COORD{ 0, -2}, COORD{ 0, -1}, COORD{NULL, NULL} },
		{ COORD{-1, -2}, COORD{NULL, NULL} },
		{ COORD{-2, -1}, COORD{-1, -1}, COORD{-2,  0}, COORD{-1,  0}, COORD{NULL, NULL} },
		{ COORD{-2,  1}, COORD{-1,  1}, COORD{-2,  2}, COORD{-1,  2}, COORD{NULL, NULL} },
		{ COORD{ 0,  2}, COORD{ 0,  1}, COORD{NULL, NULL} },
		{ COORD{ 1,  2}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};
	static const COORD right_hitting_box[][5] = {
		{ COORD{-1, -2}, COORD{NULL, NULL} },
		{ COORD{ 0, -2}, COORD{ 0, -1}, COORD{NULL, NULL} },
		{ COORD{ 1, -2}, COORD{NULL, NULL} },
		{ COORD{ 2, -1}, COORD{ 1, -1}, COORD{ 2,  0}, COORD{ 1,  0}, COORD{NULL, NULL} },
		{ COORD{ 2,  1}, COORD{ 1,  1}, COORD{ 2,  2}, COORD{ 1,  2}, COORD{NULL, NULL} },
		{ COORD{ 0,  2}, COORD{ 0,  1}, COORD{NULL, NULL} },
		{ COORD{-1,  2}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};

	/* �ǰ� ���� ���� */

	const COORD* hitting_box;
	switch (dir)
	{
	case UP:    hitting_box = &up_hitting_box[0][0];    break;
	case DOWN:  hitting_box = &down_hitting_box[0][0];  break;
	case LEFT:  hitting_box = &left_hitting_box[0][0];  break;
	case RIGHT: hitting_box = &right_hitting_box[0][0]; break;
	default: return false;
	}

	/* �÷��̾� �ǰ� ���� */

	// ��ų ��� ������ �ϳ��� ����
	for (int i = 0; (hitting_box + i)->X | (hitting_box + i)->Y; i += 5)
	{
		// �÷��̾ �ϳ��� ����
		for (std::list<Client*>::iterator iter = background.clients.begin();
			iter != background.clients.end(); iter++)
		{
			Client* client = (*iter);
			COORD client_pos = client->get_pos();

			// i��° ������ �ǰ��������� �ϳ��� �����ϸ� �÷��̾��� ��ġ��ǥ���� ��
			for (int j = 0; (hitting_box + i)[j].X | (hitting_box + i)[j].Y; j++)
			{
				COORD hitting_pos = pos;
				hitting_pos.X += (hitting_box + i)[j].X; hitting_pos.Y += (hitting_box + i)[j].Y;
			
				if (client_pos.X == hitting_pos.X &&
					client_pos.Y == hitting_pos.Y)
					client->hit(this);
			}
		}

		Sleep(delay[i / 5]); // ���� ���� [i] => [i / 5] ( ã�Ƴ� ���� �ڶ�������
	}

	return true;
}
int LightsaberSlash::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int LightsaberSlash::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
SKILL_TYPE LightsaberSlash::get_type() const
{
	return LIGHTSABER_SLASH;
}
int LightsaberSlash::get_max_level() const
{
	return MAX_LEVEL;
}
int LightsaberSlash::get_ordinal() const
{
	return 2;
}
bool LightsaberSlash::upgradable() const
{
	return false;
}
bool LightsaberSlash::upgradable_to(SKILL_TYPE type) const
{
	return false;
}

bool LightsaberSlash::downgradable() const
{
	return true;
}

bool LightsaberSlash::downgradable_to(SKILL_TYPE type) const
{
	if (type == SLASH)
		return true;

	return Slash::downgradable_to(type);
}