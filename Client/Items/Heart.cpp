#include "Heart.h"

#include "graphic.h"

extern Graphic graphic;

Heart::Heart(COORD pos) : Item(pos)
{

}

int Heart::get_amount() const
{
	return AMOUNT;
}
ITEM_TYPE Heart::get_type() const
{
	return HEART;
}

void Heart::appear() const
{
	graphic.draw(get_pos(), "\u2665", RED);
}