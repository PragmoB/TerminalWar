#include "Heart.h"

#include "graphic.h"
#include "Values/interface.h"

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
	graphic.draw(graphic.get_client_pos_by_server_pos(get_pos()), "\u2665", RED, graphic.FIELD_BACKGROUND_COLOR);
}
void Heart::disappear() const
{
	COORD client_pos = graphic.get_client_pos_by_server_pos(get_pos());
	graphic.draw(client_pos, "::", graphic.FIELD_BACKGROUND_COLOR, graphic.FIELD_BACKGROUND_COLOR);
	if (get_pos().X == FIELD_WIDTH)
		graphic.draw(COORD{ client_pos.X + 1, client_pos.Y }, ":", graphic.FIELD_FRAME_COLOR, graphic.FIELD_FRAME_COLOR);
}