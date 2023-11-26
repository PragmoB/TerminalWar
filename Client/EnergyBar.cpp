#include <cstdio>

#include "Graphic.h"
#include "EnergyBar.h"
#include "interface.h"

extern Graphic graphic;

EnergyBar::EnergyBar()
{
}
void EnergyBar::earn_energy(int amount)
{
	// 에너지 흡수
	energy += amount;

	// 에너지가 넘치면 레벨업
	if (REQUIRED_ENERGY[level] <= energy)
	{
		energy = 0;
		level++;
	}

	appear();
}
void EnergyBar::hunt_player(int player_level)
{
	earn_energy(HUNTING_ENERGY[player_level]);
}
void EnergyBar::appear()
{
	char caption[20] = "";
	sprintf_s(caption, "Lv. %d", level);

	const SHORT LEN_CAPTION = (SHORT)strlen(caption);
	const SHORT TOTAL_WIDTH = 2 * FIELD_WIDTH - LEN_CAPTION + 15;
	const SHORT VALUE_WIDTH = energy > REQUIRED_ENERGY[level] ? TOTAL_WIDTH : TOTAL_WIDTH * energy / REQUIRED_ENERGY[level];

	// caption [
	graphic.draw(COORD{ FIELD.Left - 1, FIELD.Bottom + 3 }, caption, WHITE);
	graphic.draw(COORD{ FIELD.Left - 1 + LEN_CAPTION, FIELD.Bottom + 3 }, " [", WHITE);

	// :::::::::::::::::::
	char buff[400] = "";
	for (int i = 0; i < VALUE_WIDTH; i++)
		buff[i] = ':';
	buff[VALUE_WIDTH] = NULL;
	graphic.draw(COORD{ FIELD.Left + LEN_CAPTION + 1, FIELD.Bottom + 3 }, buff, WHITE, WHITE);

	// :::::::::::::::::::
	for (int i = 0; i < TOTAL_WIDTH - VALUE_WIDTH; i++)
		buff[i] = ' ';
	buff[TOTAL_WIDTH - VALUE_WIDTH] = NULL;
	graphic.draw(COORD{ FIELD.Left + LEN_CAPTION + 1 + VALUE_WIDTH, FIELD.Bottom + 3 }, buff);

	// ]
	graphic.draw(COORD{ FIELD.Left + LEN_CAPTION + 1 + TOTAL_WIDTH, FIELD.Bottom + 3 }, "]", WHITE);

	// energy/REQUIRED_ENERGY[level]
	sprintf_s(caption, "%4d/%4d", energy, REQUIRED_ENERGY[level]);
	graphic.draw(COORD{ FIELD.Left + LEN_CAPTION + 3 + TOTAL_WIDTH, FIELD.Bottom + 3 }, caption, WHITE);
}

int EnergyBar::get_level() const
{
	return level;
}