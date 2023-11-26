#include "Heart.h"

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