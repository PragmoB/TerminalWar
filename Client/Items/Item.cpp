#include <list>

#include "graphic.h"

#include "Item.h"

extern Graphic graphic;

std::list<Item*> items;

Item::Item(COORD pos) : pos(pos)
{

}

COORD Item::get_pos() const
{
	return pos;
}

void Item::disappear() const
{
	graphic.draw(get_pos(), ' ');
}

Item::~Item()
{
}