#include "Graphic.h"
#include "Player.h"

#include "Skills/Wind.h"

extern Graphic graphic;

const int Wind::DAMAGE[] = { 60, 63, 66, 69, 72, 75, 78, 81, 85, 89 };
const int Wind::COOLDOWN[] = { 788, 710, 639, 576, 519, 468, 422, 380, 342, 308 };
const int Wind::BPS[] = { 25, 26, 27, 28, 29, 30, 31, 32, 33, 34 };
const int Wind::DISTANCE[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

Wind::Wind(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Skill(owner, level, type, MAX_LEVEL)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
	bps = BPS[level - 1];
	distance = DISTANCE[level - 1];
}

bool Wind::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	const COORD pos = owner->get_pos();
	COORD pos_temp = pos;

	switch (dir)
	{
	case UP:
	{
		// [i] = i번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD up_effects_pos[] = {
		   COORD{0, -3}, COORD{-3, -2}, COORD{-4, -1}, COORD{-5, 0}, COORD{1, 0}, COORD{-5, 1}, COORD{1, 1}, COORD{-5, 2}, COORD{4, 2}
		};

		// 스킬 이펙트 출력
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

		// 스킬 이펙트 지우기
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
		// [i] = i번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD down_effects_pos[] = {
		   COORD{0, 3}, COORD{-3, 2}, COORD{-4, 1}, COORD{-5, 0}, COORD{1, 0}, COORD{-5, -1}, COORD{1, -1}, COORD{-5, -2}, COORD{4, -2}
		};

		// 스킬 이펙트 출력
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

		// 스킬 이펙트 지우기
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
		// [i] = i번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD left_effects_pos[] = {
		   COORD{0, -3}, COORD{-3, -2}, COORD{-4, -1}, COORD{-5, 0}, COORD{-5, 1}, COORD{-4, 2}, COORD{-1, 3}
		};

		// 스킬 이펙트 출력
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

		// 스킬 이펙트 지우기
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
		// [i] = i번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD right_effects_pos[] = {
		   COORD{-2, -3}, COORD{0, -2}, COORD{2, -1}, COORD{3, 0}, COORD{3, 1}, COORD{1, 2}, COORD{-2, 3}
		};

		// 스킬 이펙트 출력
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

		// 스킬 이펙트 지우기
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
void Wind::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
}