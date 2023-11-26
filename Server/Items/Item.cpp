#include <list>

#include "Item.h"

Item::Item(COORD pos) : pos(pos)
{

}

COORD Item::get_pos() const
{
	return pos;
}