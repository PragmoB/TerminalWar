#pragma once

#define _WINSOCKAPI_
#include <windows.h>

enum ITEM_TYPE
{
	HEART, ENERGY
};

class Item
{
private:
	COORD pos;
public :
	Item(COORD pos);
	virtual ITEM_TYPE get_type() const = 0;
	COORD get_pos() const;
};