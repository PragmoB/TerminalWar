#pragma once

#include "Item.h"

class Heart : public Item
{
private:
	static const int AMOUNT;
public :
	virtual ITEM_TYPE get_type() const;
	int get_amount() const;
	Heart(COORD pos);
};