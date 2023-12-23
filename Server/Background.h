#pragma once

#include <thread>
#include <list>

#include "blocking_queue.h"

#include "Client.h"
#include "Skills/ActiveSkill.h"
#include "Items/Item.h"

typedef struct {
	ActiveSkill* skill;
	DIRECTION dir;
} SkillParam;

class Background
{
private:
	BlockingQueue<SkillParam> skill_queue;
	std::thread umpires[10];
	std::thread item_generator;

	// ��ų �ǰ����� ������
	void check_attack();

	// ������ ���� ������
	void generate_item();
public:
	std::list<Client*> clients;
	std::list<Item*> items;

	Background();

	// ��ų �ǰ����� ��û
	void cast_skill(ActiveSkill* skill, DIRECTION dir);
};