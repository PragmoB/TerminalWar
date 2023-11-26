#include "Energy.h"

Energy::Energy(int level, COORD pos) : level(level), Item(pos)
{

}

ITEM_TYPE Energy::get_type() const
{
	return ENERGY;
}
int Energy::get_amount() const
{
	return AMOUNT[level - 1];
}

int Energy::get_level() const
{
	return level;
}