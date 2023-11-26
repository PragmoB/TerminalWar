#include "Graphic.h"
#include "Player.h"

#include "Skills/Wind.h"

extern Graphic graphic;

Wind::Wind(Player* owner, int level)
	: Skill(owner, level)
{
}

bool Wind::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Player* owner = get_owner();
	const int distance = get_distance();
	const int bps = get_bps();
	const COORD pos = owner->get_pos();
	COORD pos_temp = pos;

	switch (dir)
	{
	case UP:
	{
		// [i] = i��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD up_effects_pos[] = {
		   COORD{0, -3}, COORD{-3, -2}, COORD{-4, -1}, COORD{-5, 0}, COORD{1, 0}, COORD{-5, 1}, COORD{1, 1}, COORD{-5, 2}, COORD{4, 2}
		};

		// ��ų ����Ʈ ���
		static const char* up_effects[] = {
		   "_", "-`` ``-", "``     ``", "``   ", "   ``", " `   ", "   ` ", "  ", "  ", NULL
		};
		for (int i = 0; i < distance; i++)
		{
			for (int j = 0; up_effects[j]; j++)
			{
				pos_temp.X = pos.X + up_effects_pos[j].X;
				pos_temp.Y = pos.Y + up_effects_pos[j].Y - i;

				graphic.draw_in_field(pos_temp, up_effects[j]);
			}

			Sleep(1000 / bps);
		}

		// ��ų ����Ʈ �����
		static const char* up_erase_effects[] = {
		   " ", "       ", "         ", "     ", "     ", "    ", "    ", "  ", "  ", NULL
		};
		for (int j = 0; up_erase_effects[j]; j++)
		{
			pos_temp.X = pos.X + up_effects_pos[j].X;
			pos_temp.Y = pos.Y + up_effects_pos[j].Y - distance + 1;

			graphic.draw_in_field(pos_temp, up_erase_effects[j]);
		}

		break;

	}
	case DOWN:
	{
		// [i] = i��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD down_effects_pos[] = {
		   COORD{0, 3}, COORD{-3, 2}, COORD{-4, 1}, COORD{-5, 0}, COORD{1, 0}, COORD{-5, -1}, COORD{1, -1}, COORD{-5, -2}, COORD{4, -2}
		};

		// ��ų ����Ʈ ���
		static const char* down_effects[] = {
		   "`", "-__ __-", "__     __", "__   ", "   __", " _   ", "   _ ", "  ", "  ", NULL
		};
		for (int i = 0; i < distance; i++)
		{
			for (int j = 0; down_effects[j]; j++)
			{
				pos_temp.X = pos.X + down_effects_pos[j].X;
				pos_temp.Y = pos.Y + down_effects_pos[j].Y + i;

				graphic.draw_in_field(pos_temp, down_effects[j]);
			}

			Sleep(1000 / bps);
		}

		// ��ų ����Ʈ �����
		static const char* down_erase_effects[] = {
		   " ", "       ", "         ", "     ", "     ", "    ", "    ", "  ", "  ", NULL
		};
		for (int j = 0; down_erase_effects[j]; j++)
		{
			pos_temp.X = pos.X + down_effects_pos[j].X;
			pos_temp.Y = pos.Y + down_effects_pos[j].Y + distance - 1;

			graphic.draw_in_field(pos_temp, down_erase_effects[j]);
		}
		break;

	}
	case LEFT:
	{
		// [i] = i��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD left_effects_pos[] = {
		   COORD{0, -3}, COORD{-3, -2}, COORD{-4, -1}, COORD{-5, 0}, COORD{-5, 1}, COORD{-4, 2}, COORD{-1, 3}
		};

		// ��ų ����Ʈ ���
		static const char* left_effects[] = {
		   "__ ", "-`` ", "`` ", "`` ", "`` ", "``_ ", "``` ", NULL
		};
		for (int i = 0; i < distance * 2; i++)
		{
			for (int j = 0; left_effects[j]; j++)
			{
				pos_temp.X = pos.X + left_effects_pos[j].X - i;
				pos_temp.Y = pos.Y + left_effects_pos[j].Y;

				graphic.draw_in_field(pos_temp, left_effects[j]);
			}

			Sleep(500 / bps);
		}

		// ��ų ����Ʈ �����
		static const char* left_erase_effects[] = {
		   "   ", "    ", "   ", "   ", "   ", "    ", "    ", NULL
		};
		for (int j = 0; left_erase_effects[j]; j++)
		{
			pos_temp.X = pos.X + left_effects_pos[j].X - distance * 2 + 1;
			pos_temp.Y = pos.Y + left_effects_pos[j].Y;

			graphic.draw_in_field(pos_temp, left_erase_effects[j]);
		}
		break;
	}
	case RIGHT:
	{
		// [i] = i��° ����Ʈ�� ��� ��ǥ.
		// ��� ��ǥ�� ���� COORD{0, 0}�� �߽��� ��ǥ�迡�� ǥ���� ��ǥ���̴�.
		static const COORD right_effects_pos[] = {
		   COORD{-2, -3}, COORD{0, -2}, COORD{2, -1}, COORD{3, 0}, COORD{3, 1}, COORD{1, 2}, COORD{-2, 3}
		};

		// ��ų ����Ʈ ���
		static const char* right_effects[] = {
		   " __", " ``-", " ``", " ``", " ``", " _``", " ```", NULL
		};
		for (int i = 0; i < distance * 2; i++)
		{
			for (int j = 0; right_effects[j]; j++)
			{
				pos_temp.X = pos.X + right_effects_pos[j].X + i;
				pos_temp.Y = pos.Y + right_effects_pos[j].Y;

				graphic.draw_in_field(pos_temp, right_effects[j]);
			}

			Sleep(500 / bps);
		}

		// ��ų ����Ʈ �����
		static const char* right_erase_effects[] = {
		   "   ", "    ", "   ", "   ", "   ", "    ", "    ", NULL
		};
		for (int j = 0; right_erase_effects[j]; j++)
		{
			pos_temp.X = pos.X + right_effects_pos[j].X + distance * 2 - 1;
			pos_temp.Y = pos.Y + right_effects_pos[j].Y;

			graphic.draw_in_field(pos_temp, right_erase_effects[j]);
		}
		break;
	}
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
const char* Wind::get_skill_name() const
{
	return "ǳ��";
}
void Wind::get_level_up_message(char* output, int len) const
{
	const char* skill_name = Wind::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[��Ƽ��] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[��Ƽ��] %-16s : ���ݷ� %d%% ���� | ��Ÿ�� %d%% ���� | ź�� %d%%���� | ��Ÿ� %dĭ ����",
			skill_name,
			100 * (DAMAGE[LEVEL] - DAMAGE[LEVEL - 1]) / DAMAGE[LEVEL - 1],
			100 * (COOLDOWN[LEVEL - 1] - COOLDOWN[LEVEL]) / COOLDOWN[LEVEL - 1],
			100 * (BPS[LEVEL] - BPS[LEVEL - 1]) / BPS[LEVEL - 1],
			DISTANCE[LEVEL] - DISTANCE[LEVEL - 1]);
	}
}
void Wind::get_learn_message(char* output, int len) const
{
	const char* skill_name = Wind::get_skill_name();

	sprintf_s(output, len, "[��Ƽ��] %-16s : ���濡 �˱⸦ �����ϴ�. | ���ݷ� %d | ��Ÿ�� %d.%d��",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000);
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