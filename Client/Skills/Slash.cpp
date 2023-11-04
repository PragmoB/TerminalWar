#include "Graphic.h"
#include "Player.h"

#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

extern Graphic graphic;

const int Slash::DAMAGE[] = { 100, 105, 110, 115, 120 };
const int Slash::COOLDOWN[] = { 1200, 1080, 972, 875, 788 };

Slash::Slash(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Skill(owner, level, type, MAX_LEVEL)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
}

bool Slash::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	const COORD pos = owner->get_pos();
	COORD pos_temp = pos;

	static const int frames_delay[] = { 30, 30, 20, 10, 10, 10, 150, 0, NULL };

	switch (dir)
	{
	case UP:
	{
		// [i][j] = i��° �������� j��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD up_frames_pos[][9] = {
		   { COORD{-3, 1} },
		   { COORD{-4, 1}, COORD{-5, 0} },
		   { COORD{-4, 0}, COORD{-4, -1}, COORD{-3, -2} },
		   { COORD{-1, -1}, COORD{-2, -2}, COORD{0, -3} },
		   { COORD{0, -2}, COORD{0, -1} },
		   { COORD{2, -2}, COORD{1, -1}, COORD{1, 0} },
		   { COORD{1, 0}, COORD{1, 1} },
		   { COORD{-4, 1}, COORD{-5, 0}, COORD{-4, -1}, COORD{-3, -2}, COORD{0, -3}, COORD{1, 0} }
		};
		// [i][j] = i��° �������� j��° ����Ʈ
		static const char* up_frames[][10] = {
		   { "_-`", NULL },
		   { "`   ", "`----", NULL },
		   { "`   ", "`` \\", "-\\", NULL },
		   { " |", "``|", "_", NULL },
		   { " `/", " /", NULL },
		   { "`-", "  ``", "----`", NULL},
		   { "   `", "`-_`", NULL },
		   { "         ", "     ", "         ", "       ", " ", "     ", NULL },
		   { NULL }
		};


		for (int i = 0; up_frames[i][0]; i++)
		{
			for (int j = 0; up_frames[i][j]; j++)
			{
				pos_temp.X = pos.X + up_frames_pos[i][j].X;
				pos_temp.Y = pos.Y + up_frames_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, up_frames[i][j]);
			}

			Sleep(frames_delay[i]);
		}
		break;

	}
	case DOWN:
	{
		// [i][j] = i��° �������� j��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD down_frames_pos[][9] = {
		   { COORD{-3, -1} },
		   { COORD{-4, -1}, COORD{-5, 0} },
		   { COORD{-4, 0}, COORD{-4, 1}, COORD{-3, 2} },
		   { COORD{-1, 1}, COORD{-2, 2}, COORD{0, 3} },
		   { COORD{0, 2}, COORD{0, 1} },
		   { COORD{2, 2}, COORD{1, 1}, COORD{1, 0} },
		   { COORD{1, 0}, COORD{1, -1} },
		   { COORD{-4, -1}, COORD{-5, 0}, COORD{-4, 1}, COORD{-3, 2}, COORD{0, 3}, COORD{1, 0} }
		};
		// [i][j] = i��° �������� j��° ����Ʈ
		static const char* down_frames[][10] = {
		   { "`-_", NULL },
		   { "_   ", "_----", NULL },
		   { "_   ", "__ /", "_/", NULL },
		   { " |", "__|", "`", NULL },
		   { " _\\", " \\", NULL },
		   { "_-", "  __", "----`", NULL},
		   { "   _", "_-`_", NULL },
		   { "         ", "     ", "         ", "       ", " ", "     ", NULL },
		   { NULL }
		};
		for (int i = 0; down_frames[i][0]; i++)
		{
			for (int j = 0; down_frames[i][j]; j++)
			{
				pos_temp.X = pos.X + down_frames_pos[i][j].X;
				pos_temp.Y = pos.Y + down_frames_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, down_frames[i][j]);
			}

			Sleep(frames_delay[i]);
		}
		break;

	}
	case LEFT:
	{
		/* ��ų �ִϸ��̼� ����. ����)��ų �ִϸ��̼��� �׳� �߳�ٰ��̹Ƿ� �����Ұ� */

		// [i][j] = i��° �������� j��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD left_frames_pos[][9] = {
		   { COORD{1, -1}, COORD{2, -2} },
		   { COORD{1, -1}, COORD{2, -2}, COORD{0, -3}, COORD{0, -2}, COORD{0, -1} },
		   { COORD{0, -2}, COORD{0, -1}, COORD{-2, -2}, COORD{-1, -1} },
		   { COORD{-2, -2}, COORD{-1, -1}, COORD{-3, -2}, COORD{-4, -1}, COORD{-5, 0} },
		   { COORD{-4, 0}, COORD{-5, 1}, COORD{-4, 2} },
		   { COORD{-1, 1}, COORD{-2, 2}, COORD{0, 1}, COORD{-2, 2}, COORD{-1, 3} },
		   { COORD{0, 1}, COORD{0, 2}, COORD{0, 3}, COORD{1, 1}, COORD{2, 2} },
		   { COORD{0, -3}, COORD{-3, -2}, COORD{-4, -1}, COORD{-5, 0}, COORD{-5, 1}, COORD{-4, 2}, COORD{-1, 3} }
		};
		// [i][j] = i��° �������� j��° ����Ʈ
		static const char* left_frames[][10] = {
		   { "/", "/", NULL },
		   { " ", " ", "__", "|", "|", NULL },
		   { " ", " ", "\\`", "\\", NULL },
		   { " ", " ", "-`", "``", "`----", NULL },
		   { "`   ", "``  /", "``/", NULL },
		   { " ", " ", "|", "_ |", "`", NULL },
		   { " ", " ", "``", "\\", "\\", NULL },
		   { "  ", "   ", "  ", "  ", "       ", "       ", "   " },
		   { NULL }
		};

		for (int i = 0; left_frames[i][0]; i++)
		{
			for (int j = 0; left_frames[i][j]; j++)
			{
				pos_temp.X = pos.X + left_frames_pos[i][j].X;
				pos_temp.Y = pos.Y + left_frames_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, left_frames[i][j]);
			}

			Sleep(frames_delay[i]);
		}
		break;
	}
	case RIGHT:
	{
		static const COORD right_frames_pos[][9] = {
		   { COORD{-1, -1}, COORD{-2, -2} },
		   { COORD{-1, -1}, COORD{-2, -2}, COORD{-1, -3}, COORD{0, -2}, COORD{0, -1} },
		   { COORD{0, -2}, COORD{0, -1}, COORD{1, -2}, COORD{1, -1} },
		   { COORD{1, -2}, COORD{1, -1}, COORD{1, -2}, COORD{3, -1}, COORD{1, 0} },
		   { COORD{1, 0}, COORD{1, 1}, COORD{2, 2} },
		   { COORD{1, 1}, COORD{2, 2}, COORD{0, 1}, COORD{0, 2}, COORD{1, 3} },
		   { COORD{0, 1}, COORD{0, 2}, COORD{-1, 3}, COORD{-1, 1}, COORD{-2, 2} },
		   { COORD{-1, -3}, COORD{1, -2}, COORD{3, -1}, COORD{4, 0}, COORD{-1, 1}, COORD{-2, 2}, COORD{-1, 3} }
		};
		static const char* right_frames[][10] = {
		   { "\\", "\\", NULL },
		   { " ", " ", "__", "|", "|", NULL },
		   { " ", " ", "`/", "/", NULL },
		   { " ", " ", "``-", "``", "----`", NULL },
		   { "   `", "\\  ``", "\\``", NULL },
		   { " ", " ", "|", "| _", "`", NULL },
		   { " ", " ", "``", "/", "/", NULL },
		   { "  ", "   ", "  ", "  ", "       ", "       ", "   " },
		   { NULL }
		};

		for (int i = 0; right_frames[i][0]; i++)
		{
			for (int j = 0; right_frames[i][j]; j++)
			{
				pos_temp.X = pos.X + right_frames_pos[i][j].X;
				pos_temp.Y = pos.Y + right_frames_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, right_frames[i][j]);
			}

			Sleep(frames_delay[i]);
		}
		break;
	}
	}
	return true;
}
void Slash::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
}