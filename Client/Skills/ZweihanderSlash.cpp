#include "Graphic.h"
#include "Player.h"

#include "Skills/ZweihanderSlash.h"

extern Graphic graphic;

const int ZweihanderSlash::DAMAGE[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
const int ZweihanderSlash::COOLDOWN[] = { 170, 170, 170, 170, 170, 170, 170, 170, 170, 170 };

ZweihanderSlash::ZweihanderSlash(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
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

	COORD pos_temp;

	pos_temp = pos;

	switch (dir)
	{
	case UP:
	{
		static const int up_frames_delay[] = { 50, 1, 1, 1, 1, 1, 150, 0 };
		// [i][j] = i번째 프레임의 j번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
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
		// [i][j] = i번째 프레임의 j번째 이펙트
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

			Sleep(up_frames_delay[i]);
		}
		break;

	}
	case DOWN:
	{
		static const int down_frames_delay[] = { 50, 1, 1, 1, 1, 1, 150, 0 };
		// [i][j] = i번째 프레임의 j번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
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
		// [i][j] = i번째 프레임의 j번째 이펙트
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

			Sleep(down_frames_delay[i]);
		}
		break;

	}
	case LEFT:
	{
		/* 스킬 애니메이션 정의. 참고)스킬 애니메이션은 그냥 쌩노다가이므로 참고할것 */

		// [i] = i번째 프레임의 딜레이
		static const int left_frames_delay[] = { 50, 1, 1, 1, 1, 1, 150, 0 };
		// [i][j] = i번째 프레임의 j번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
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
		// [i][j] = i번째 프레임의 j번째 이펙트
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

			Sleep(left_frames_delay[i]);
		}
		break;
	}
	case RIGHT:
	{
		static const int right_frames_delay[] = { 50, 1, 1, 1, 1, 1, 150, 0 };
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

			Sleep(right_frames_delay[i]);
		}
		break;
	}
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