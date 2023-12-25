#include <list>

#include "Background.h"
#include "ZweihanderSlash.h"

extern Background background;

ZweihanderSlash::ZweihanderSlash(Player* owner, int level)
	: Slash(owner, level)
{
}

bool ZweihanderSlash::cast(DIRECTION dir)
{
	if (!ActiveSkill::cast(dir))
		return false;

	Player* owner = get_owner();
	const COORD pos = owner->get_pos();

	static const int delay[] = { 50, 20, 20, 10, 10, 10, 0, NULL };
	owner->bind(120);

	/* pos�� �������� �ǰ� ����(hitting box) ���� */

	static const COORD up_hitting_box[][9] = {
		{ COORD{-3,  2}, COORD{-2,  1}, COORD{-1,  1}, COORD{NULL, NULL} },
		{ COORD{-3,  1}, COORD{-3,  0}, COORD{-2,  0}, COORD{-1,  0}, COORD{NULL, NULL} },
		{ COORD{-3, -1}, COORD{-2, -1}, COORD{-1, -1}, COORD{-3, -2}, COORD{-2, -2}, COORD{-1, -2}, COORD{-2, -3}, COORD{NULL, NULL} },
		{ COORD{ 0, -1}, COORD{ 0, -2}, COORD{-1, -3}, COORD{ 0, -3}, COORD{ 0, -4}, COORD{NULL, NULL} },
		{ COORD{ 1, -2}, COORD{ 1, -3}, COORD{NULL, NULL} },
		{ COORD{ 3, -3}, COORD{ 3, -2}, COORD{ 1, -1}, COORD{ 2, -1}, COORD{ 3, -1}, COORD{ 1,  0}, COORD{ 2,  0}, COORD{ 3,  0}, COORD{NULL, NULL} },
		{ COORD{ 1,  1}, COORD{ 2,  1}, COORD{ 3,  1}, COORD{ 3,  2}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};
	static const COORD down_hitting_box[][9] = {
		{ COORD{-3, -2}, COORD{-2, -1}, COORD{-1, -1}, COORD{NULL, NULL} },
		{ COORD{-3, -1}, COORD{-3,  0}, COORD{-2,  0}, COORD{-1,  0}, COORD{NULL, NULL} },
		{ COORD{-3,  1}, COORD{-2,  1}, COORD{-1,  1}, COORD{-3,  2}, COORD{-2,  2}, COORD{-1,  2}, COORD{-2,  3}, COORD{NULL, NULL} },
		{ COORD{ 0,  1}, COORD{ 0,  2}, COORD{-1,  3}, COORD{ 0,  3}, COORD{ 0,  4}, COORD{NULL, NULL} },
		{ COORD{ 1,  2}, COORD{ 1,  3}, COORD{NULL, NULL} },
		{ COORD{ 3,  3}, COORD{ 3,  2}, COORD{ 1,  1}, COORD{ 2,  1}, COORD{ 3,  1}, COORD{ 1,  0}, COORD{ 2,  0}, COORD{ 3,  0}, COORD{NULL, NULL} },
		{ COORD{ 1, -1}, COORD{ 2, -1}, COORD{ 3, -1}, COORD{ 3, -2}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};
	static const COORD left_hitting_box[][9] = {
		{ COORD{ 1, -2}, COORD{ 2, -3}, COORD{NULL, NULL} },
		{ COORD{ 0, -1}, COORD{ 0, -2}, COORD{ 0, -3}, COORD{ 1, -3}, COORD{ 0, -4}, COORD{NULL, NULL} },
		{ COORD{-1, -2}, COORD{-1, -3}, COORD{NULL, NULL} },
		{ COORD{-3,  0}, COORD{-2,  0}, COORD{-1,  0}, COORD{-3, -1}, COORD{-2, -1}, COORD{-1, -1}, COORD{-2, -2}, COORD{-2, -3}, COORD{NULL, NULL} },
		{ COORD{-3,  1}, COORD{-2,  1}, COORD{-1,  1}, COORD{-3,  2}, COORD{-2,  2}, COORD{-1,  2}, COORD{-2,  3}, COORD{NULL, NULL} },
		{ COORD{ 0,  1}, COORD{ 0,  2}, COORD{ 0,  3}, COORD{-1,  3}, COORD{NULL, NULL} },
		{ COORD{ 1,  2}, COORD{ 1,  3}, COORD{NULL, NULL} },
		{ COORD{NULL, NULL} }
	};
	static const COORD right_hitting_box[][9] = {
		{ COORD{-1, -2}, COORD{-2, -3}, COORD{NULL, NULL} },
		{ COORD{ 0, -1}, COORD{ 0, -2}, COORD{ 0, -3}, COORD{-1, -3}, COORD{ 0, -4}, COORD{NULL, NULL} },
		{ COORD{ 1, -2}, COORD{ 1, -3}, COORD{NULL, NULL} },
		{ COORD{ 3,  0}, COORD{ 2,  0}, COORD{ 1,  0}, COORD{ 3, -1}, COORD{ 2, -1}, COORD{ 1, -1}, COORD{ 2, -2}, COORD{ 2, -3}, COORD{NULL, NULL} },
		{ COORD{ 3,  1}, COORD{ 2,  1}, COORD{ 1,  1}, COORD{ 3,  2}, COORD{ 2,  2}, COORD{ 1,  2}, COORD{ 2,  3}, COORD{NULL, NULL} },
		{ COORD{ 0,  1}, COORD{ 0,  2}, COORD{ 0,  3}, COORD{ 1,  3}, COORD{NULL, NULL} },
		{ COORD{-1,  2}, COORD{-1,  3}, COORD{NULL, NULL} },
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
	for (int i = 0; (hitting_box + i)->X | (hitting_box + i)->Y; i += 9)
	{
		// �÷��̾ �ϳ��� ����
		for (std::list<Player*>::iterator iter = background.players.begin();
			iter != background.players.end(); iter++)
		{
			Player* player = (*iter);
			COORD player_pos = player->get_pos();

			// i��° ������ �ǰ��������� �ϳ��� �����ϸ� �÷��̾��� ��ġ��ǥ���� ��
			for (int j = 0; (hitting_box + i)[j].X | (hitting_box + i)[j].Y; j++)
			{
				COORD hitting_pos = pos;
				hitting_pos.X += (hitting_box + i)[j].X; hitting_pos.Y += (hitting_box + i)[j].Y;

				if (player_pos.X == hitting_pos.X &&
					player_pos.Y == hitting_pos.Y)
					player->hit(this);
			}
		}

		Sleep(delay[i / 9]);
	}

	return true;
}

int ZweihanderSlash::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int ZweihanderSlash::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
SKILL_TYPE ZweihanderSlash::get_type() const
{
	return ZWEIHANDER_SLASH;
}
int ZweihanderSlash::get_max_level() const
{
	return MAX_LEVEL;
}
int ZweihanderSlash::get_ordinal() const
{
	return 2;
}
bool ZweihanderSlash::upgradable() const
{
	return false;
}
bool ZweihanderSlash::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool ZweihanderSlash::downgradable() const
{
	return true;
}

bool ZweihanderSlash::downgradable_to(SKILL_TYPE type) const
{
	if (type == SLASH)
		return true;

	return Slash::downgradable_to(type);
}