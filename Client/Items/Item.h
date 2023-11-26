#pragma once

#include <windows.h>

enum ITEM_TYPE
{
	HEART, ENERGY
};

class Item
{
private:
	COORD pos;
public:
	Item(COORD pos);
	virtual ITEM_TYPE get_type() const = 0;
	virtual void appear() const = 0;
	void disappear() const;
	COORD get_pos() const;
	~Item();
};