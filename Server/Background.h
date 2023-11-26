#pragma once

#include <thread>
#include <list>

#include "blocking_queue.h"

#include "Client.h"
#include "Skills/Skill.h"
#include "Items/Item.h"

typedef struct {
	Skill* skill;
	DIRECTION dir;
} SkillParam;

class Background
{
private:
	BlockingQueue<SkillParam> skill_queue;
	std::thread umpires[10];
	std::thread item_generator;

	// 스킬 피격판정 스레드
	void check_attack();

	// 아이템 생성 스레드
	void generate_item();
public:
	std::list<Client*> clients;
	std::list<Item*> items;

	Background();

	// 스킬 피격판정 요청
	void cast_skill(Skill* skill, DIRECTION dir);
};