#pragma once

#include "Item.h"

class Energy : public Item
{
private:
	static const int AMOUNT[3];
	int level;
public:

	Energy(int level, COORD pos);
	virtual ITEM_TYPE get_type() const;
	virtual void appear() const;
	int get_amount() const;
	int get_level() const;
};