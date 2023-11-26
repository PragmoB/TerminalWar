#include "Graphic.h"
#include "Player.h"

#include "Skills/ZweihanderSlash.h"

extern Graphic graphic;

ZweihanderSlash::ZweihanderSlash(Player* owner, int level)
	: Slash(owner, level)
{
}

bool ZweihanderSlash::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Player* owner = get_owner();
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
		   { COORD{-4, 1}, COORD{-6, 2} },
		   { COORD{-5, 2}, COORD{-7, 1}, COORD{-7, 0} },
		   { COORD{-6, 0}, COORD{-7, -1}, COORD{-6, -2}, COORD{-5, -3} },
		   { COORD{-1, -1}, COORD{-2, -2}, COORD{-3, -3}, COORD{-3, -4} },
		   { COORD{0, -1}, COORD{0, -2}, COORD{0, -3}, COORD{1, -4} },
		   { COORD{1, 0}, COORD{1, -1}, COORD{2, -2}, COORD{3, -3} },
		   { COORD{1, 0}, COORD{1, 1}, COORD{5, 2} },
		   { COORD{-3, -4}, COORD{-5, -3}, COORD{-6, -2}, COORD{-7, -1}, COORD{-7, 0}, COORD{1, 0}, COORD{-7, 1}, COORD{-6, 2} }
		};
		// [i][j] = i��° �������� j��° ����Ʈ
		static const char* up_frames[][10] = {
		   { "_-``", "``", NULL },
		   { " ", "`      ", "`------", NULL },
		   { "      ", "``    \\", "``  \\", "--\\", NULL },
		   { " |", "  |", "```|", "__--", NULL },
		   { " /", "  /", "```/", "-__", NULL },
		   { "------`", "     ``", "   ``", "`--", NULL },
		   { "      ", "``-_  `", "``", NULL },
		   { "       ", "           ", "             ", "               ", "       ", "       ", "               ", "             " },
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
		   { COORD{-4, -1}, COORD{-6, -2} },
		   { COORD{-5, -2}, COORD{-7, -1}, COORD{-7, 0} },
		   { COORD{-6, 0}, COORD{-7, 1}, COORD{-6, 2}, COORD{-5, 3} },
		   { COORD{-1, 1}, COORD{-2, 2}, COORD{-3, 3}, COORD{-3, 4} },
		   { COORD{0, 1}, COORD{0, 2}, COORD{0, 3}, COORD{1, 4} },
		   { COORD{1, 0}, COORD{1, 1}, COORD{2, 2}, COORD{3, 3} },
		   { COORD{1, 0}, COORD{1, -1}, COORD{5, -2} },
		   { COORD{-3, 4}, COORD{-5, 3}, COORD{-6, 2}, COORD{-7, 1}, COORD{-7, 0}, COORD{1, 0}, COORD{-7, -1}, COORD{-6, -2} }
		};
		// [i][j] = i��° �������� j��° ����Ʈ
		static const char* down_frames[][10] = {
		   { "`-__", "__", NULL },
		   { " ", "_      ", "_------", NULL },
		   { "      ", "__    /", "__  /", "--/", NULL },
		   { " |", "  |", "___|", "``--", NULL },
		   { " \\", "  \\", "___\\", "-``", NULL },
		   { "------_", "     __", "   __", "_--", NULL },
		   { "      ", "__-`  _", "__", NULL },
		   { "       ", "           ", "             ", "               ", "       ", "       ", "               ", "             " },
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
		   { COORD{1, -1}, COORD{2, -2}, COORD{3, -3} },
		   { COORD{0, -1}, COORD{0, -2}, COORD{0, -3}, COORD{0, -4} },
		   { COORD{-3, -4}, COORD{-3, -3}, COORD{-2, -2}, COORD{-1, -1} },
		   { COORD{-5, -3}, COORD{-6, -2}, COORD{-7, -1}, COORD{-8, 0} },
		   { COORD{-6, 0}, COORD{-8, 1}, COORD{-7, 2}, COORD{-6, 3} },
		   { COORD{-1, 1}, COORD{-2, 2}, COORD{-3, 3}, COORD{-3, 4} },
		   { COORD{0, 1}, COORD{0, 2}, COORD{0, 3}, COORD{1, 4} },
		   { COORD{-3, -4}, COORD{-5, -3}, COORD{-6, -2}, COORD{-7, -1}, COORD{-8, 0}, COORD{-8, 1}, COORD{-7, 2}, COORD{-6, 3}, COORD{-3, 4} }
		};
		// [i][j] = i��° �������� j��° ����Ʈ
		static const char* left_frames[][10] = {
		   { "/", "/", "/`", NULL },
		   { "| ", "|  ", "|   ", "--__", NULL },
		   { "__-", "\\   ", "\\  ", "\\ ", NULL },
		   { "-``", "``   ", "``     ", "``------", NULL },
		   { "`     ", "```    /", "`--  /", "`-_/", NULL },
		   { " |", "  |", "_  |", "``--", NULL },
		   { " \\", "  \\", "   \\", "--`", NULL },
		   { "       ", "          ", "  ", "  ", "   ", "          ", "          ", "          ", "       ", NULL },
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
		   { COORD{-1, -1}, COORD{-2, -2}, COORD{-4, -3} },
		   { COORD{-1, -1}, COORD{-2, -2}, COORD{-3, -3}, COORD{-3, -4} },
		   { COORD{1, -4}, COORD{0, -3}, COORD{0, -2}, COORD{0, -1} },
		   { COORD{3, -3}, COORD{2, -2}, COORD{1, -1}, COORD{1, 0} },
		   { COORD{1, 0}, COORD{1, 1}, COORD{2, 2}, COORD{3, 3} },
		   { COORD{0, 1}, COORD{0, 2}, COORD{0, 3}, COORD{0, 4} },
		   { COORD{-1, 1}, COORD{-2, 2}, COORD{-3, 3}, COORD{-3, 4} },
		   { COORD{-3, -4}, COORD{-4, -3}, COORD{5, -2}, COORD{6, -1}, COORD{6, 0}, COORD{-1, 1}, COORD{-2, 2}, COORD{-3, 3}, COORD{-3, 4} }
		};
		static const char* right_frames[][10] = {
		   { "\\", "\\", "`\\", NULL },
		   { " |", "  |", "   |", "__--", NULL },
		   { "-__", "   /", "  /", " /", NULL },
		   { "``-", "   ``", "     ``", "------``", NULL },
		   { "     `", "\\    ```", "\\  --`", "\\_-`", NULL },
		   { "| ", "|  ", "|  _", "--``", NULL },
		   { "/ ", "/  ", "/   ", "`--", NULL },
		   { "       ", "          ", "  ", "  ", "   ", "          ", "          ", "          ", "       ", NULL },
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
const char* ZweihanderSlash::get_skill_name() const
{
	return "����ˡ�����";
}
void ZweihanderSlash::get_level_up_message(char* output, int len) const
{
	const char* skill_name = ZweihanderSlash::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[��Ƽ��] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[��Ƽ��] %-16s : ���ݷ� %d%% ���� | ��Ÿ�� %d%% ����",
			skill_name,
			100 * (DAMAGE[LEVEL + 1] - DAMAGE[LEVEL]) / DAMAGE[LEVEL],
			100 * (COOLDOWN[LEVEL] - COOLDOWN[LEVEL + 1]) / COOLDOWN[LEVEL]);
	}
}
void ZweihanderSlash::get_learn_message(char* output, int len) const
{
	const char* skill_name = ZweihanderSlash::get_skill_name();

	sprintf_s(output, len, "[��Ƽ��] %-16s : ���� ���ݹ����� ��հ��� �ֵθ��ϴ�. | ���ݷ� %d | ��Ÿ�� %d.%d��",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000);
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