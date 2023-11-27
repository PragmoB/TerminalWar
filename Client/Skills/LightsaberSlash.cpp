#include "Graphic.h"
#include "Player.h"
#include "Skills/LightsaberSlash.h"

extern Graphic graphic;

LightsaberSlash::LightsaberSlash(Player* owner, int level)
	: Slash(owner, level)
{	
}

bool LightsaberSlash::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Player* owner = get_owner();
	const COORD pos = owner->get_pos();
	COORD pos_temp = pos;

	static const COLOR color = YELLOW;
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

				graphic.draw_in_field(pos_temp, up_frames[i][j], color);
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

				graphic.draw_in_field(pos_temp, down_frames[i][j], color);
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

				graphic.draw_in_field(pos_temp, left_frames[i][j], color);
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

				graphic.draw_in_field(pos_temp, right_frames[i][j], color);
			}

			Sleep(frames_delay[i]);
		}
		break;
	}
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
const char* LightsaberSlash::get_skill_name() const
{
	return "�����ˡ�����";
}
void LightsaberSlash::get_level_up_message(char* output, int len) const
{
	const char* skill_name = LightsaberSlash::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[��Ƽ��] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[��Ƽ��] %-16s : ���ݷ� %d%% ���� | ��Ÿ�� %d%% ����",
			skill_name,
			100 * (DAMAGE[LEVEL] - DAMAGE[LEVEL - 1]) / DAMAGE[LEVEL - 1],
			100 * (COOLDOWN[LEVEL - 1] - COOLDOWN[LEVEL]) / COOLDOWN[LEVEL - 1]);
	}
}
void LightsaberSlash::get_learn_message(char* output, int len) const
{
	const char* skill_name = LightsaberSlash::get_skill_name();

	sprintf_s(output, len, "[��Ƽ��] %-16s : ������ �������� �������� �ֵθ��ϴ�. | ���ݷ� %d | ��Ÿ�� %d.%d��",
		skill_name,	
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000);
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