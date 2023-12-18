#include <list>

#include "graphic.h"

#include "Item.h"

extern Graphic graphic;

std::list<Item*> items;

Item::Item(COORD pos)
{
	this->pos = graphic.get_client_pos_by_server_pos(pos);
}

COORD Item::get_pos() const
{
	return graphic.get_server_pos_by_client_pos(pos);
}

void Item::disappear() const
{
	graphic.draw(pos, ':', graphic.FIELD_BACKGROUND_COLOR, graphic.FIELD_BACKGROUND_COLOR);
}

Item::~Item()
{
}