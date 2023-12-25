#pragma once

#define _WINSOCKAPI_
#include <Windows.h>
#include <vector>

class UpgradeOptionList
{
private:
	COORD pos;
	char* title;
	std::vector<char*> contents_list;
public:
	UpgradeOptionList(COORD pos, const char* title);
	void push_content(const char* content);
	void appear();
	void disappear();
	void clear();
	size_t get_size();
};