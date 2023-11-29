#include "LightningStrike.h"

#include "graphic.h"
#include "Player.h"

extern Graphic graphic;

LightningStrike::LightningStrike(Player* owner, int level)
	: Skill(owner, level)
{
}

bool LightningStrike::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	const COORD pos = get_owner()->get_pos();
	COORD pos_temp;
	pos_temp = pos;

	static const int energy_drain_frame_delay[] = { 50, 0, 50, 0, 50, 0, 50, 0, 150, 0 };

	static const COORD energy_drain_frame_pos[][11] = {
	{ COORD{-2, -5}, COORD{ -7, -4 }, COORD{ -9, -3 }, COORD{ -10, -2 }, COORD{ -10, -1 }, COORD{ -10, 0 }, COORD{ -10, 1 }, COORD{ -10, 2 }, COORD{ -9, 3 }, COORD{ -6, 4 } },
	{ COORD{-2, -5}, COORD{ -7, -4 }, COORD{ -9, -3 }, COORD{ -10, -2 }, COORD{ -10, -1 }, COORD{ -10, 0 }, COORD{ -10, 1 }, COORD{ -10, 2 }, COORD{ -9, 3 }, COORD{ -6, 4 } },
	{ COORD{ -2, -4 }, COORD{-6, -3}, COORD{-8, -2}, COORD{-8, -1}, COORD{-8, 0}, COORD{-8, 1}, COORD{-8, 2}, COORD{-6, 3} },
	{ COORD{ -2, -4 }, COORD{-6, -3}, COORD{-8, -2}, COORD{-8, -1}, COORD{-8, 0}, COORD{-8, 1}, COORD{-8, 2}, COORD{-6, 3} },
	{ COORD{ -1, -3 }, COORD{-4, -2}, COORD{-5, -1}, COORD{-5, 0}, COORD{-5, 1}, COORD{-4, 2} },
	{ COORD{ -1, -3 }, COORD{-4, -2}, COORD{-5, -1}, COORD{-5, 0}, COORD{-5, 1}, COORD{-4, 2} },
	{ COORD{ -2, -2 }, COORD{-3, -1}, COORD{-4, 0}, COORD{-3, 1}, COORD{-2, 2} },
	{ COORD{ -2, -2 }, COORD{-3, -1}, COORD{-4, 0}, COORD{-3, 1}, COORD{-2, 2} },
	{ COORD{ -3, -1 }, COORD{-3, 0}, COORD{-2, 1} },
	{ COORD{ -3, -1 }, COORD{-3, 0}, COORD{-2, 1} }
	};

	static const char* energy_drain_frame[][25] = {
	   { "_____",
	   "_,--`     `--,_",
	   ",/`             `\\,",
	   ",|                 |,",
	   "|                   |",
	   "|                   |",
	   "|                   |",
	   "`|                 |`",
	   "`\\,             ,/`",
	   "`--,_____,--`",
	   NULL },

	   { "     ",
	   "               ",
	   "                   ",
	   "                     ",
	   "                     ",
	   "                     ",
	   "                     ",
	   "                     ",
	   "                   ",
	   "             ",
	   NULL },

	   { "_____",
	   ",--`     `--,",
	   ",/             \\,",
	   "|               |",
	   "|               |",
	   "|               |",
	   "`\\,           ,/`",
	   "`--,_____,--`",
	   NULL },

	   { "     ",
	   "             ",
	   "                 ",
	   "                 ",
	   "                 ",
	   "                 ",
	   "                 ",
	   "             ",
	   NULL },

	   { "___",
	   ",-`   `-,",
	   "/`       `\\",
	   "|         |",
	   "\\,       ,/",
	   "`-,___,-`",
	   NULL },

	   { "   ",
	   "         ",
	   "           ",
	   "           ",
	   "           ",
	   "         ",
	   NULL },

	   { ",---,",
	   "/`   `\\",
	   "|       |",
	   "\\,   ,/",
	   "`---`",
	   NULL },

	   { "     ",
	   "       ",
	   "         ",
	   "       ",
	   "     ",
	   NULL },

	   { ",-``-,",
	   ":    :",
	   "`--`",
	   NULL },

	   { "      ",
	   "      ",
	   "    ",
	   NULL },

	   { NULL }
	};



	switch (dir)
	{
	case UP:
	{
		static const int up_frames_delay[] = { 15, 15, 15, 15, 15, 0, 45, 0, 45, 0, 45, 0, 45, 0, 200, 0 };
		// [i][j] = i번째 프레임의 j번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD up_frames_pos[][17] = {
		   { COORD{ 0, -13 }, COORD{ -12, -12 }, COORD{ -11, -11 }, COORD{ 0, -10 }, COORD{ 0, -9 }, COORD{ 0, -8 }, COORD{ 0, -7 }, COORD{ 0, -6 }, COORD{ 0, -5 }, COORD{ 0, -4 }, COORD{ 0, -3 }, COORD{ 0, -2 }, COORD{ 0, -1 } },
		   { COORD{ -11, -15 }, COORD{ -12, -14 }, COORD{ -12, -13 }, COORD{ -12, -12 }, COORD{ -11, -11 }, COORD{ 0, -10 }, COORD{ 0, -9 }, COORD{ 0, -8 }, COORD{ 0, -7 }, COORD{ 0, -6 }, COORD{ 0, -5 }, COORD{ -12, -4 }, COORD{ -11, -3 }, COORD{ 0, -2 }, COORD{ 0, -1 } },
		   { COORD{ -8, -16 }, COORD{ -11, -15 }, COORD{ -12, -14 }, COORD{ -12, -13 }, COORD{ -12, -12 }, COORD{ -11, -11 }, COORD{ 0, -10 }, COORD{ 0, -9 }, COORD{ -8, -8 }, COORD{ -11, -7 }, COORD{ -12, -6 }, COORD{ -12, -5 }, COORD{ -12, -4 }, COORD{ -11, -3 }, COORD{ 0, -2 }, COORD{ 0, -1 } },
		   { COORD{ -8, -16 }, COORD{ -11, -15 }, COORD{ -12, -14 }, COORD{ -12, -13 }, COORD{ -12, -12 }, COORD{ -11, -11 }, COORD{ 0, -10 }, COORD{ 0, -9 }, COORD{ -8, -8 }, COORD{ -11, -7 }, COORD{ -12, -6 }, COORD{ -12, -5 }, COORD{ -12, -4 }, COORD{ -11, -3 }, COORD{ 0, -2 }, COORD{ 0, -1 } },
		   { COORD{ -8, -16 }, COORD{ -11, -15 }, COORD{ -12, -14 }, COORD{ -12, -13 }, COORD{ -12, -12 }, COORD{ -11, -11 }, COORD{ 0, -10 }, COORD{ 0, -9 }, COORD{ -8, -8 }, COORD{ -11, -7 }, COORD{ -12, -6 }, COORD{ -12, -5 }, COORD{ -12, -4 }, COORD{ -11, -3 }, COORD{ 0, -2 }, COORD{ 0, -1 } },
		   { COORD{ -8, -16 }, COORD{ -11, -15 }, COORD{ -12, -14 }, COORD{ -12, -13 }, COORD{ -12, -12 }, COORD{ -11, -11 }, COORD{ 0, -10 }, COORD{ 0, -9 }, COORD{ -8, -8 }, COORD{ -11, -7 }, COORD{ -12, -6 }, COORD{ -12, -5 }, COORD{ -12, -4 }, COORD{ -11, -3 }, COORD{ 0, -2 }, COORD{ 0, -1 } },
		   { COORD{  0, -14 }, COORD{-1, -13}, COORD{2, -12}, COORD{3, -11}, COORD{3, -10}, COORD{2, -9}, COORD{1, -8}, COORD{0, -7}, COORD{-1, -6} },
		   { COORD{  0, -14 }, COORD{-1, -13}, COORD{2, -12}, COORD{3, -11}, COORD{3, -10}, COORD{2, -9}, COORD{1, -8}, COORD{0, -7}, COORD{-1, -6} },
		   { COORD{ -1, -16 }, COORD{0, -15}, COORD{-3, -14}, COORD{-3, -13}, COORD{-3, -12}, COORD{0, -9}, COORD{3, -8}, COORD{4, -7}, COORD{4, -6}, COORD{3, -5}, COORD{0, -3}, COORD{0, -2} },
		   { COORD{ -1, -16 }, COORD{0, -15}, COORD{-3, -14}, COORD{-3, -13}, COORD{-3, -12}, COORD{0, -9}, COORD{3, -8}, COORD{4, -7}, COORD{4, -6}, COORD{3, -5}, COORD{0, -3}, COORD{0, -2} },
		   { COORD{ -1, -16 }, COORD{0, -15}, COORD{0, -14}, COORD{-1, -13}, COORD{-4, -12}, COORD{-3, -11}, COORD{-3, -10}, COORD{-3, -9}, COORD{3, -6}, COORD{4, -5}, COORD{4, -4}, COORD{4, -3}, COORD{3, -2}, COORD{1, -1} },
		   { COORD{ -1, -16 }, COORD{0, -15}, COORD{0, -14}, COORD{-1, -13}, COORD{-4, -12}, COORD{-3, -11}, COORD{-3, -10}, COORD{-3, -9}, COORD{3, -6}, COORD{4, -5}, COORD{4, -4}, COORD{4, -3}, COORD{3, -2}, COORD{1, -1} },
		   { COORD{  0, -16 }, COORD{0, -15}, COORD{0, -14}, COORD{0, -13}, COORD{ 0, -12 }, COORD{ 1, -11 }, COORD{ 3, -10 }, COORD{ 2, -9 }, COORD{ -6, -7 }, COORD{ -1, -6 }, COORD{ -2, -5 }, COORD{ -1, -4 }, COORD{ 0, -3 }, COORD{ 1, -2 }, COORD{ 0, -1 } },
		   { COORD{  0, -16 }, COORD{0, -15}, COORD{0, -14}, COORD{0, -13}, COORD{ 0, -12 }, COORD{ 1, -11 }, COORD{ 3, -10 }, COORD{ 2, -9 }, COORD{ -6, -7 }, COORD{ -1, -6 }, COORD{ -2, -5 }, COORD{ -1, -4 }, COORD{ 0, -3 }, COORD{ 1, -2 }, COORD{ 0, -1 } },
		   { COORD{  0, -16 }, COORD{0, -15}, COORD{ 0, -14 }, COORD{0, -13}, COORD{-4, -12}, COORD{0, -11}, COORD{0, -10}, COORD{0, -9}, COORD{-6, -8}, COORD{0, -7}, COORD{0, -6}, COORD{0, -5}, COORD{0, -4}, COORD{-4, -3}, COORD{0, -2}, COORD{0, -1} },
		   { COORD{  0, -16 }, COORD{0, -15}, COORD{ 0, -14 }, COORD{0, -13}, COORD{-4, -12}, COORD{0, -11}, COORD{0, -10}, COORD{0, -9}, COORD{-6, -8}, COORD{0, -7}, COORD{0, -6}, COORD{0, -5}, COORD{0, -4}, COORD{-4, -3}, COORD{0, -2}, COORD{0, -1} }
		};
		// [i][j] = i번째 프레임의 j번째 이펙트
		static const char* up_frames[][35] = {
		   { "|",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   NULL},

		   { ",                     ,",
		   "/`                     `\\",
		   "|           |           |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "|",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   NULL},

		   { "___           ___",
		   ",-`   `\\,     ,/`   `-,",
		   "/`                     `\\",
		   "|           |           |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   "|",
		   ",          |          ,",
		   "/`          |          `\\",
		   "|           |           |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   NULL},

		   { "___           ___",
		   ",-`   `\\,     ,/`   `-,",
		   "/`   /`  |     |  `\\   `\\",
		   "|    \\__/`  |  `\\__/    |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   "___           ___",
		   ",-`   `\\,     ,/`   `-,",
		   "/`                     `\\",
		   "|           |           |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   NULL},

		   { "___           ___",
		   ",-`   `\\,     ,/`   `-,",
		   "/`   /`  |     |  `\\   `\\",
		   "|    \\__/`  |  `\\__/    |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   "___           ___",
		   ",-`   `\\,     ,/`   `-,",
		   "/`   /`  |     |  `\\   `\\",
		   "|    \\__/`  |  `\\__/    |",
		   "\\,          |          ,/",
		   "`-,___,`   |   `,___,-`",
		   "|",
		   "|",
		   NULL},

		   { "                 ",
		   "                       ",
		   "                         ",
		   "                         ",
		   "                         ",
		   "                       ",
		   " ",
		   " ",
		   "                 ",
		   "                       ",
		   "                         ",
		   "                         ",
		   "                         ",
		   "                       ",
		   " ",
		   " ",
		   NULL},

		   { "|`",
		   ",|",
		   "\\,",
		   "|",
		   "|",
		   "/`",
		   ",",
		   "|",
		   "`",
		   NULL },

		   { "  ",
		   "  ",
		   "  ",
		   " ",
		   " ",
		   "  ",
		   " ",
		   " ",
		   " ",
		   NULL },

		   { "`|",
		   "|,",
		   ",/",
		   "|",
		   "`\\",
		   "\\,",
		   ",",
		   "|",
		   "|",
		   "`",
		   ",/",
		   "/`",
		   NULL },

		   { "  ",
		   "  ",
		   "  ",
		   " ",
		   "  ",
		   "  ",
		   " ",
		   " ",
		   " ",
		   " ",
		   "  ",
		   "  ",
		   NULL },

		   { ",  +",
		   "|",
		   "|",
		   "`",
		   "+",
		   ",/",
		   "|",
		   "`\\",
		   ",",
		   "|",
		   "|",
		   "|",
		   "`",
		   ",/",
		   NULL },

		   { "    ",
		   " ",
		   " ",
		   " ",
		   " ",
		   "  ",
		   " ",
		   "  ",
		   " ",
		   " ",
		   " ",
		   " ",
		   " ",
		   "  ",
		   NULL },

		   { "-",
		   "-",
		   "-   +",
		   "-",
		   "- ,",
		   "` |",
		   "|",
		   "`",
		   "+     ,",
		   "/",
		   "|",
		   "\\    +",
		   "`,",
		   "/",
		   "`",
		   NULL },

		   { " ",
		   " ",
		   "     ",
		   " ",
		   "   ",
		   "   ",
		   " ",
		   " ",
		   "       ",
		   " ",
		   " ",
		   "      ",
		   "  ",
		   " ",
		   " ",
		   NULL },

		   { "-",
		   "-",
		   "-   +",
		   "-",
		   "+   -",
		   "-",
		   "-",
		   "-    +",
		   "`\\,   -",
		   "-",
		   "-",
		   "-    ,/`",
		   "-",
		   "+   -",
		   "-",
		   "-",
		   NULL },

		   { " ",
		   " ",
		   "     ",
		   " ",
		   "     ",
		   " ",
		   " ",
		   "      ",
		   "       ",
		   " ",
		   " ",
		   "        ",
		   " ",
		   "     ",
		   " ",
		   " ",
		   NULL },

		   { NULL }
		};

		for (int i = 0; energy_drain_frame[i][0]; i++)
		{
			for (int j = 0; energy_drain_frame[i][j]; j++)
			{
				pos_temp.X = pos.X + energy_drain_frame_pos[i][j].X;
				pos_temp.Y = pos.Y + energy_drain_frame_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, energy_drain_frame[i][j]);
			}

			Sleep(energy_drain_frame_delay[i]);
		}

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
		static const int down_frames_delay[] = { 15, 15, 15, 15, 15, 0, 45, 0, 45, 0, 45, 0, 45, 0, 200, 0 };
		// [i][j] = i번째 프레임의 j번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD down_frames_pos[][17] = {
		   { COORD{ 0, 1 }, COORD{ 0, 2 }, COORD{ 0, 3 }, COORD{ 0, 4 }, COORD{ 0, 5 }, COORD{ 0, 6 }, COORD{ 0, 7 }, COORD{ 0, 8 }, COORD{ 0, 9 }, COORD{ -7, 10 }, COORD{ -10, 11 }, COORD{ -11, 12 }, COORD{ 0, 13 }  },
		   { COORD{ 0, 1 }, COORD{ -7, 2 }, COORD{ -10, 3 }, COORD{ -11, 4 }, COORD{ 0, 5 }, COORD{ 0, 6 }, COORD{ 0, 7 }, COORD{ 0, 8 }, COORD{ 0, 9 }, COORD{ -7, 10 }, COORD{ -10, 11 }, COORD{ -11, 12 }, COORD{ -11, 13 }, COORD{ -11, 14 }, COORD{ -10, 15 } },
		   { COORD{ 0, 1 }, COORD{ -7, 2 }, COORD{ -10, 3 }, COORD{ -11, 4 }, COORD{ -11, 5 }, COORD{ -11, 6 }, COORD{ -10, 7 }, COORD{ 0, 8 }, COORD{ 0, 9 }, COORD{ -7, 10 }, COORD{ -10, 11 }, COORD{ -11, 12 }, COORD{ -11, 13 }, COORD{ -11, 14 }, COORD{ -10, 15 } },
		   { COORD{ 0, 1 }, COORD{ -7, 2 }, COORD{ -10, 3 }, COORD{ -11, 4 }, COORD{ -11, 5 }, COORD{ -11, 6 }, COORD{ -10, 7 }, COORD{ 0, 8 }, COORD{ 0, 9 }, COORD{ -7, 10 }, COORD{ -10, 11 }, COORD{ -11, 12 }, COORD{ -11, 13 }, COORD{ -11, 14 }, COORD{ -10, 15 } },
		   { COORD{ 0, 1 }, COORD{ -7, 2 }, COORD{ -10, 3 }, COORD{ -11, 4 }, COORD{ -11, 5 }, COORD{ -11, 6 }, COORD{ -10, 7 }, COORD{ 0, 8 }, COORD{ 0, 9 }, COORD{ -7, 10 }, COORD{ -10, 11 }, COORD{ -11, 12 }, COORD{ -11, 13 }, COORD{ -11, 14 }, COORD{ -10, 15 } },
		   { COORD{ 0, 1 }, COORD{ -7, 2 }, COORD{ -10, 3 }, COORD{ -11, 4 }, COORD{ -11, 5 }, COORD{ -11, 6 }, COORD{ -10, 7 }, COORD{ 0, 8 }, COORD{ 0, 9 }, COORD{ -7, 10 }, COORD{ -10, 11 }, COORD{ -11, 12 }, COORD{ -11, 13 }, COORD{ -11, 14 }, COORD{ -10, 15 } },
		   { COORD{ 0, 6 }, COORD{1, 7}, COORD{0, 8}, COORD{2, 10}, COORD{3, 11}, COORD{3, 12}, COORD{2, 13}, COORD{-1, 14}, COORD{0, 15} },
		   { COORD{ 0, 6 }, COORD{1, 7}, COORD{0, 8}, COORD{2, 10}, COORD{3, 11}, COORD{3, 12}, COORD{2, 13}, COORD{-1, 14}, COORD{0, 15} },
		   { COORD{ 0, 2 }, COORD{0, 3}, COORD{3, 5}, COORD{4, 6}, COORD{4, 7}, COORD{3, 8}, COORD{0, 9}, COORD{-3, 12}, COORD{-3, 13}, COORD{-3, 14}, COORD{0, 15}, COORD{-1, 16} },
		   { COORD{ 0, 2 }, COORD{0, 3}, COORD{3, 5}, COORD{4, 6}, COORD{4, 7}, COORD{3, 8}, COORD{0, 9}, COORD{-3, 12}, COORD{-3, 13}, COORD{-3, 14}, COORD{0, 15}, COORD{-1, 16} },
		   { COORD{ 0, 1 }, COORD{3, 2}, COORD{-6, 3}, COORD{4, 4}, COORD{4, 5}, COORD{3, 6}, COORD{-5, 9}, COORD{-5, 10}, COORD{-5, 11}, COORD{-2, 13}, COORD{-1, 14}, COORD{-1, 15}, COORD{-2, 16} },
		   { COORD{ 0, 1 }, COORD{3, 2}, COORD{-6, 3}, COORD{4, 4}, COORD{4, 5}, COORD{3, 6}, COORD{-5, 9}, COORD{-5, 10}, COORD{-5, 11}, COORD{-2, 13}, COORD{-1, 14}, COORD{-1, 15}, COORD{-2, 16} },
		   { COORD{ 0, 1 }, COORD{1, 2}, COORD{0, 3}, COORD{-1, 4}, COORD{-2, 5}, COORD{-4, 6}, COORD{-8, 7}, COORD{2, 9}, COORD{3, 10}, COORD{3, 11}, COORD{1, 12}, COORD{0, 13}, COORD{0, 14}, COORD{0, 15}, COORD{0, 16} },
		   { COORD{ 0, 1 }, COORD{1, 2}, COORD{0, 3}, COORD{-1, 4}, COORD{-2, 5}, COORD{-4, 6}, COORD{-8, 7}, COORD{2, 9}, COORD{3, 10}, COORD{3, 11}, COORD{1, 12}, COORD{0, 13}, COORD{0, 14}, COORD{0, 15}, COORD{0, 16} },
		   { COORD{ 0, 1 }, COORD{-3, 2}, COORD{0, 3}, COORD{0, 4}, COORD{0, 5}, COORD{0, 6}, COORD{0, 7}, COORD{-6, 8}, COORD{0, 9}, COORD{0, 10}, COORD{0, 11}, COORD{0, 12}, COORD{0, 13}, COORD{0, 14}, COORD{-5, 15}, COORD{0, 14} },
		   { COORD{ 0, 1 }, COORD{-3, 2}, COORD{0, 3}, COORD{0, 4}, COORD{0, 5}, COORD{0, 6}, COORD{0, 7}, COORD{-6, 8}, COORD{0, 9}, COORD{0, 10}, COORD{0, 11}, COORD{0, 12}, COORD{0, 13}, COORD{0, 14}, COORD{-5, 15}, COORD{0, 14} }
		};
		// [i][j] = i번째 프레임의 j번째 이펙트
		static const char* down_frames[][30] = {
		{ "|",
		"|",
		"|",
		"|",
		"|",
		"|",
		"|",
		"|",
		"|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/          |          \\",
		"|",
		   NULL },

		{ "|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/          |          \\",
		"|",
		"|",
		"|",
		"|",
		"|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/          |          \\",
		"|          |          |",
		"\\                     /",
		"`                   `",
		   NULL },

		{ "|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/          |          \\",
		"|",
		"|",
		"|",
		"|",
		"|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/          |          \\",
		"|          |          |",
		"\\                     /",
		"`                   `",
		   NULL },

		{ "|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/          |          \\",
		"|          |          |",
		"\\          |          /",
		"`-,__,/   |   \\,__,-`",
		"|",
		"|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/    __    |    __    \\",
		"|    /  \\   |   /  \\    |",
		"\\   \\,  |     |  ,/   /",
		"`-,__,/       \\,__,-`",
		   NULL },

		{ "|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/    __    |    __    \\",
		"|    /  \\   |   /  \\    |",
		"\\   \\,  |     |  ,/   /",
		"`-,__,/       \\,__,-`",
		"|",
		"|",
		"____   |   ____",
		",-`    `  |  `    `-,",
		"/    __    |    __    \\",
		"|    /  \\   |   /  \\    |",
		"\\   \\,  |     |  ,/   /",
		"`-,__,/       \\,__,-`",
		   NULL },

		{ " ",
		"               ",
		"                     ",
		"                        ",
		"                         ",
		"                       ",
		"                     ",
		" ",
		" ",
		"               ",
		"                     ",
		"                       ",
		"                         ",
		"                       ",
		"                     ",
		   NULL },

		{ ",",
		"|",
		"`",
		"\\,",
		"|",
		"|",
		"/`",
		"`|",
		"|,",
		NULL },

		{ " ",
		" ",
		" ",
		"  ",
		" ",
		" ",
		"  ",
		"  ",
		"  ",
		NULL },

		{ "\\,",
		"`\\",
		",",
		"|",
		"|",
		"`",
		"/`",
		",/",
		"|",
		"`\\",
		"|`",
		",|",
		NULL },

		{ "  ",
		"  ",
		" ",
		" ",
		" ",
		" ",
		"  ",
		"  ",
		" ",
		"  ",
		"  ",
		"  ",
		NULL },

		{ "`\\,",
		",",
		"+         |",
		"|",
		"|",
		"`",
		",/",
		"|       +",
		"`\\",
		",",
		"|",
		"|   +",
		"`",
		NULL },

		{ "   ",
		" ",
		"           ",
		" ",
		" ",
		" ",
		"  ",
		"         ",
		"   ",
		" ",
		" ",
		"     ",
		" ",
		NULL },

		{ ",",
		"\\",
		",`  +",
		"/",
		"|",
		"+       \\",
		",",
		"|",
		"|",
		",`",
		"-",
		"-  +",
		"-",
		"-",
		"-",
		NULL },

		{ " ",
		" ",
		"     ",
		" ",
		" ",
		"         ",
		" ",
		" ",
		" ",
		"  ",
		" ",
		"    ",
		" ",
		" ",
		" ",
		NULL },

		{ "-",
		"+  -",
		"-   ,/`",
		"-",
		"-",
		"-",
		"-",
		"`\\,   -",
		"-",
		"-   +",
		"-",
		"-",
		"-",
		"-",
		"+    -",
		"-",
		NULL },

		{ " ",
		"    ",
		"       ",
		" ",
		" ",
		" ",
		" ",
		"       ",
		" ",
		"     ",
		" ",
		" ",
		" ",
		" ",
		"      ",
		" ",
		NULL },

		{ NULL }
		};

		for (int i = 0; energy_drain_frame[i][0]; i++)
		{
			for (int j = 0; energy_drain_frame[i][j]; j++)
			{
				pos_temp.X = pos.X + energy_drain_frame_pos[i][j].X;
				pos_temp.Y = pos.Y + energy_drain_frame_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, energy_drain_frame[i][j]);
			}

			Sleep(energy_drain_frame_delay[i]);
		}

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
		static const int left_frames_delay[] = { 15, 15, 15, 15, 15, 0, 45, 0, 45, 0, 45, 0, 45, 0, 200, 0 };
		static const COORD left_frames_pos[][15] = {
		   { COORD{ -25, -3 }, COORD{ -27, -2 }, COORD{ -28, -1 }, COORD{ -30, 0 }, COORD{ -28, 1 }, COORD{ -27, 2 }, COORD{ -25, 3 } },
		   { COORD{ -30, -6 }, COORD{-28, -5}, COORD{-27, -4}, COORD{-25, -3}, COORD{-27, -2}, COORD{-28, -1}, COORD{-30, 0}, COORD{-28, 1}, COORD{-27, 2}, COORD{-25, 3}, COORD{-27, 4}, COORD{-30, 5} },
		   { COORD{ -32, -6 }, COORD{-34, -5}, COORD{-35, -4}, COORD{-25, -3}, COORD{-27, -2}, COORD{-28, -1}, COORD{-30, 0}, COORD{-28, 1}, COORD{-27, 2}, COORD{-25, 3}, COORD{-35, 4}, COORD{-33, 5} },
		   { COORD{ -32, -6 }, COORD{-34, -5}, COORD{-35, -4}, COORD{-35, -3}, COORD{-27, -2}, COORD{-28, -1}, COORD{-30, 0}, COORD{-28, 1}, COORD{-33, 2}, COORD{-35, 3}, COORD{-35, 4}, COORD{-33, 5} },
		   { COORD{ -32, -6 }, COORD{-34, -5}, COORD{-35, -4}, COORD{-35, -3}, COORD{-27, -2}, COORD{-28, -1}, COORD{-30, 0}, COORD{-28, 1}, COORD{-33, 2}, COORD{-35, 3}, COORD{-35, 4}, COORD{-33, 5} },
		   { COORD{ -32, -6 }, COORD{-34, -5}, COORD{-35, -4}, COORD{-35, -3}, COORD{-27, -2}, COORD{-28, -1}, COORD{-30, 0}, COORD{-28, 1}, COORD{-33, 2}, COORD{-35, 3}, COORD{-35, 4}, COORD{-33, 5} },
		   { COORD{ -30, 0 }, COORD{-27, 1} },
		   { COORD{ -30, 0 }, COORD{-27, 1} },
		   { COORD{ -16, -2 }, COORD{-40, -1}, COORD{-27, 0} },
		   { COORD{ -16, -2 }, COORD{-40, -1}, COORD{-27, 0} },
		   { COORD{ -44, -2 }, COORD{-42, -1}, COORD{-38, 0} },
		   { COORD{ -44, -2 }, COORD{-42, -1}, COORD{-38, 0} },
		   { COORD{ -7, -2 }, COORD{-22, -1}, COORD{-28, 0}, COORD{-13, 1} },
		   { COORD{ -7, -2 }, COORD{-22, -1}, COORD{-28, 0}, COORD{-13, 1} },
		   { COORD{ -6, -3 }, COORD{-28, -2}, COORD{-10, -1}, COORD{-36, 0}, COORD{-25, 1}, COORD{-34, 2}, COORD{-27, 3} },
		   { COORD{ -6, -3 }, COORD{-28, -2}, COORD{-10, -1}, COORD{-36, 0}, COORD{-25, 1}, COORD{-34, 2}, COORD{-27, 3} }
		};

		static const char* left_frames[][40] = {
		   { "|",
		   ",/`",
		   "-`",
		   "==============================",
		   "-,",
		   "`\\",
		   "|",
		   NULL},

		   { "__",
		   "`,",
		   "`\\",
		   "|               |",
		   ",/`             ,/`",
		   "-`              -`",
		   "==============================",
		   "-,              -,",
		   "`\\              `\\",
		   "|               |",
		   ",/`",
		   "_,-`",
		   NULL },

		   { "____              __",
		   ",`    `,              `,",
		   "/`      `\\              `\\",
		   "|               |",
		   ",/`             ,/`",
		   "-`              -`",
		   "==============================",
		   "-,              -,",
		   "`\\              `\\",
		   "|               |",
		   "\\,      ,/`             ,/`",
		   "`-__,-`            _,-`",
		   NULL },

		   { "____            ____",
		   ",`    `,        ,`    `,",
		   "/`  \\   `\\      /       `\\",
		   "\\,__/     |               |",
		   ",/`             ,/`",
		   "-`              -`",
		   "==========================",
		   "-,              -,",
		   "__    `\\              `\\",
		   "/`  \\     |               |",
		   "\\,  /   ,/`     \,      ,/`",
		   "`-__,-`         `-__,-`",
		   NULL },

		   { "____            ____",
		   ",`    `,        ,`    `,",
		   "/`  \\   `\\      /   \\   `\\",
		   "\\,__/     |     \\,__/     |",
		   ",/`             ,/`",
		   "-`              -`",
		   "==========================",
		   "-,              -,",
		   "__    `\\        __    `\\",
		   "/`  \\     |     /`  \\     |",
		   "\\,  /   ,/`     \\,  /   ,/`",
		   "`-__,-`         `-__,-`",
		   NULL },

		   { "                    ",
		   "                        ",
		   "                          ",
		   "                           ",
		   "                   ",
		   "                  ",
		   "                          ",
		   "                  ",
		   "                        ",
		   "                           ",
		   "                           ",
		   "                       ",
		   NULL },

		   { "`\\,       ,----,   -",
		   "\\____/`     `--`",
		   NULL },

		   { "                    ",
		   "                ",
		   NULL },

		   { ",-----,",
		   "`\\,       --,      /`          \\,",
		   "\\______/`                `\\",
		   NULL },

		   { "       ",
		   "                                 ",
		   "                           ",
		   NULL },

		   { "+                     ,--------,",
		   ",----,       ,-`             `\\,",
		   "+ `     `\\___/`",
		   NULL },

		   { "                                ",
		   "                                ",
		   "               ",
		   NULL },

		   { "+",
		   "+  ,----,        ,-`\\,",
		   "------------------`     `\\___/`",
		   "+",
		   NULL },

		   { " ",
		   "                      ",
		   "                                 ",
		   " ",
		   NULL },

		   { "+",
		   "+        ,/`",
		   "+",
		   "------------------------------------",
		   "`\\,                ,/`",
		   "+               +",
		   "+           `\\,",
		   NULL },

		   { " ",
		   "            ",
		   " ",
		   "                                    ",
		   "                      ",
		   "                 ",
		   "               ",
		   NULL },

		   { NULL }
		};

		for (int i = 0; energy_drain_frame[i][0]; i++)
		{
			for (int j = 0; energy_drain_frame[i][j]; j++)
			{
				pos_temp.X = pos.X + energy_drain_frame_pos[i][j].X;
				pos_temp.Y = pos.Y + energy_drain_frame_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, energy_drain_frame[i][j]);
			}

			Sleep(energy_drain_frame_delay[i]);
		}

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
		/* 스킬 애니메이션 정의. 참고)스킬 애니메이션은 그냥 쌩노다가이므로 참고할것 */

		// [i] = i번째 프레임의 딜레이
		static const int right_frames_delay[] = { 15, 15, 15, 15, 15, 0, 45, 0, 45, 0, 45, 0, 45, 0, 200, 0 };
		// [i][j] = i번째 프레임의 j번째 이펙트의 상대 좌표.
		// 상대 좌표라 함은 COORD{0, 0}이 중심인 좌표계에서 표현된 좌표값이다.
		static const COORD right_frames_pos[][13] = {
		{ COORD{ 25 , -3 }, COORD{ 25, -2 }, COORD{ 27, -1 }, COORD{ 1, 0 }, COORD{ 27, 1 }, COORD{ 26, 2 }, COORD{ 25, 3 } },
		{ COORD{ 29, -6 }, COORD{27, -5}, COORD{26, -4}, COORD{9, -3}, COORD{9, -2}, COORD{11, -1}, COORD{1, 0}, COORD{11, 1}, COORD{10, 2}, COORD{9, 3}, COORD{25, 4}, COORD{27, 5} },
		{ COORD{ 13, -6 }, COORD{11, -5}, COORD{10, -4}, COORD{9, -3}, COORD{9, -2}, COORD{11, -1}, COORD{1, 0}, COORD{11, 1}, COORD{10, 2}, COORD{9, 3}, COORD{9, 4}, COORD{11, 5} },
		{ COORD{ 13, -6 }, COORD{11, -5}, COORD{10, -4}, COORD{9, -3}, COORD{9, -2}, COORD{11, -1}, COORD{1, 0}, COORD{11, 1}, COORD{10, 2}, COORD{9, 3}, COORD{9, 4}, COORD{11, 5} },
		{ COORD{ 13, -6 }, COORD{11, -5}, COORD{10, -4}, COORD{9, -3}, COORD{9, -2}, COORD{11, -1}, COORD{1, 0}, COORD{11, 1}, COORD{10, 2}, COORD{9, 3}, COORD{9, 4}, COORD{11, 5} },
		{ COORD{ 13, -6 }, COORD{11, -5}, COORD{10, -4}, COORD{9, -3}, COORD{9, -2}, COORD{11, -1}, COORD{1, 0}, COORD{11, 1}, COORD{10, 2}, COORD{9, 3}, COORD{9, 4}, COORD{11, 5} },
		{ COORD{ 11, -1 }, COORD{12, 0} },
		{ COORD{ 11, -1 }, COORD{12, 0} },
		{ COORD{ 13,-2 }, COORD{11, -1}, COORD{10, 0} },
		{ COORD{ 13,-2 }, COORD{11, -1}, COORD{10, 0} },
		{ COORD{ 3, -2 }, COORD{1, -1}, COORD{13, 0} },
		{ COORD{ 3, -2 }, COORD{1, -1}, COORD{13, 0} },
		{ COORD{ 6, -2 }, COORD{1, -1}, COORD{5, 0}, COORD{12, 1} },
		{ COORD{ 6, -2 }, COORD{1, -1}, COORD{5, 0}, COORD{12, 1} },
		{ COORD{ 4, -3 }, COORD{ 13, -2 }, COORD{ 1, 0 }, COORD{ 5, 1 }, COORD{ 13, 2 }, COORD{ 26, 3 } },
		{ COORD{ 4, -3 }, COORD{ 13, -2 }, COORD{ 1, 0 }, COORD{ 5, 1 }, COORD{ 13, 2 }, COORD{ 26, 3 } }
		};
		// [i][j] = i번째 프레임의 j번째 이펙트
		static const char* right_frames[][38] = {
		{ "|",
		"`\\,",
		"`-",
		"==============================",
		",-",
		"/`",
		"|",
		NULL},

		{ "__",
		",`",
		"/`",
		"|               |",
		"`\\,             `\\,",
		"`-              `-",
		"==============================",
		",-              ,-",
		"/`              /`",
		"|               |",
		"`\,",
		"`-,_",
		NULL },

		{ "__              ____",
		",`              ,`    `,",
		"/`              /`      `\\",
		"|               |",
		"`\\,             `\\,",
		"`-              `-",
		"==============================",
		",-              ,-",
		"/`              /`",
		"|               |",
		"`\\,             `\\,     ,/",
		"`-,_            `-,__-`",
		NULL },

		{ "____            ____",
		",`    `,        ,`    `,",
		"/`              /`   /  `\\",
		"|               |     \\__,/",
		"`\\,             `\\,",
		"`-              `-",
		"==============================",
		",-              ,-",
		"/`              /`   __",
		"|               |    /  `\\",
		"`\\,     ,/      `\\,  \\  ,/",
		"`-,__-`         `-,__-`",
		NULL },

		{ "____            ____",
		",`    `,        ,`    `,",
		"/`   /  `\\      /`   /  `\\",
		"|     \\__,/     |     \\__,/",
		"`\\,             `\\,",
		"`-              `-",
		"==============================",
		",-              ,-",
		"/`   __         /`   __",
		"|    /  `\\      |    /  `\\",
		"`\\,  \\  ,/      `\\,  \\  ,/",
		"`-,__-`         `-,__-`",
		NULL },

		{ "                    ",
		"                        ",
		"                          ",
		"                           ",
		"                   ",
		"                  ",
		"                              ",
		"                  ",
		"                       ",
		"                          ",
		"                          ",
		"                       ",
		NULL },

		{ "_    ,-----,       ,/`",
		"`--`      `\\____/",
		NULL },

		{ "                      ",
		"                 ",
		NULL },

		{ ",------,",
		",/         `\\      --,       ,/`",
		"/`               `\\______/",
		NULL },

		{ "        ",
		"                                ",
		"                          ",
		NULL },

		{ ",-------,       +           +",
		",/`            `-,       ,----,",
		"`\\___/`     ` +",
		NULL },

		{ "                             ",
		"                               ",
		"               ",
		NULL },

		{ "+",
		",/`-,       ,----,   +",
		"`\\___/`     `-------------------",
		"+",
		NULL },

		{ " ",
		"                      ",
		"                                  ",
		" ",
		NULL },

		{ "+                    +",
		",/`              ,/`",
		"------------------------------------",
		"`\\,             +             +",
		"+",
		"+",
		NULL },

		{ "                      ",
		"                    ",
		"                                    ",
		"                               ",
		" ",
		" ",
		NULL },

		{ NULL }

		};

		for (int i = 0; energy_drain_frame[i][0]; i++)
		{
			for (int j = 0; energy_drain_frame[i][j]; j++)
			{
				pos_temp.X = pos.X + energy_drain_frame_pos[i][j].X;
				pos_temp.Y = pos.Y + energy_drain_frame_pos[i][j].Y;

				graphic.draw_in_field(pos_temp, energy_drain_frame[i][j]);
			}

			Sleep(energy_drain_frame_delay[i]);
		}

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


int LightningStrike::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int LightningStrike::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
SKILL_TYPE LightningStrike::get_type() const
{
	return LIGHTNING_STRIKE;
}
int LightningStrike::get_max_level() const
{
	return MAX_LEVEL;
}
const char* LightningStrike::get_skill_name() const
{
	return "초전도화";
}
void LightningStrike::get_level_up_message(char* output, int len) const
{
	const char* skill_name = LightningStrike::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[액티브] %s 진화", skill_name);
	else
	{
		sprintf_s(output, len, "[액티브] %-16s : 공격력 %d%% 증가 | 쿨타임 %d%% 감소",
			skill_name,
			100 * (DAMAGE[LEVEL] - DAMAGE[LEVEL - 1]) / DAMAGE[LEVEL - 1],
			100 * (COOLDOWN[LEVEL - 1] - COOLDOWN[LEVEL]) / COOLDOWN[LEVEL - 1]);
	}
}
void LightningStrike::get_learn_message(char* output, int len) const
{
	const char* skill_name = LightningStrike::get_skill_name();

	sprintf_s(output, len, "[액티브] %-16s : 주변의 에너지를 끌어들여 빠르게 돌진하고, 지나간 자리에 피해를 입힙니다 | 공격력 %d | 쿨타임 %d.%d초",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000);
}
int LightningStrike::get_ordinal() const
{
	return 1;
}
bool LightningStrike::upgradable() const
{
	return false;
}
bool LightningStrike::upgradable_to(SKILL_TYPE type) const
{
	return false;
}