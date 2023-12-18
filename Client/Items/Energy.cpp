#include "Energy.h"

#include "graphic.h"

extern Graphic graphic;

Energy::Energy(int level, COORD pos) : level(level), Item(pos)
{

}

ITEM_TYPE Energy::get_type() const
{
	return ENERGY;
}

void Energy::appear() const
{
	COLOR color;

	switch (level)
	{
	case 1: color = DARK_YELLOW;   break;
	case 2: color = SKY_BLUE; break;
	case 3: color = PURPLE;   break;
	}

	graphic.draw(graphic.get_client_pos_by_server_pos(get_pos()), ':', color, color);
}
int Energy::get_amount() const
{
	return AMOUNT[level - 1];
}

int Energy::get_level() const
{
	return level;
}