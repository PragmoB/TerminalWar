#include <cstring>

#include "Graphic.h"
#include "UpgradeOptionList.h"

extern Graphic graphic;

UpgradeOptionList::UpgradeOptionList(COORD pos, const char* title) : pos(pos)
{
	this->title = new char[strlen(title) + 1];
	strcpy_s(this->title, strlen(title) + 1, title);
}

void UpgradeOptionList::push_content(const char* content)
{
	char* new_content = new char[strlen(content) + 1];
	contents_list.push_back(new_content);
	strcpy_s(new_content, strlen(content) + 1, content);
	appear();
}

void UpgradeOptionList::appear()
{
	graphic.draw(pos, title);
	for (int i = 0; i < contents_list.size(); i++)
	{
		COORD pos_temp = pos;
		pos_temp.Y += i + 1;
		char buff[10] = "";
		sprintf_s(buff, "%d. ", i + 1);

		graphic.draw(pos_temp, buff);
		pos_temp.X += strlen(buff);
		graphic.draw(pos_temp, contents_list[i]);
	}
}

void UpgradeOptionList::disappear()
{
	// 가장 긴 콘텐츠의 길이 구하기
	size_t max_content_len = strlen(title);
	for (int i = 0; i < contents_list.size(); i++)
	{
		size_t content_len = strlen(contents_list[i]);
		if (max_content_len < content_len)
			max_content_len = content_len;
	}

	// 지우개 문자열 구성하기
	char eraser[200];
	memset(eraser, ' ', sizeof(eraser));
	eraser[max_content_len + 4] = NULL;

	// 덮어버리기
	for (int i = 0; i < contents_list.size() + 1; i++)
	{
		COORD pos_temp = pos;
		pos_temp.Y += i;
		graphic.draw(pos_temp, eraser);
	}
}

void UpgradeOptionList::clear()
{
	disappear();

	size_t size = contents_list.size();
	for (size_t i = size; 0 < i; i--)
		delete contents_list[i - 1];

	contents_list.clear();
}
size_t UpgradeOptionList::get_size()
{
	return contents_list.size();
}