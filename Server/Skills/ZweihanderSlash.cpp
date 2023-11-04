#include <list>

#include "Client.h"
#include "Skills/ZweihanderSlash.h"

extern std::list<Client*> clients;

const int ZweihanderSlash::DAMAGE[] = { 120, 126, 132, 138, 144, 151, 158, 165, 173, 181 };
const int ZweihanderSlash::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };

ZweihanderSlash::ZweihanderSlash(Client* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Slash(owner, level, type, MAX_LEVEL)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
}

bool ZweihanderSlash::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

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
		for (std::list<Client*>::iterator iter = clients.begin();
			iter != clients.end(); iter++)
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

		Sleep(delay[i / 9]);
	}

	return true;
}
void ZweihanderSlash::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
}