#pragma once

#include <thread>

#include "blocking_queue.h"
#include "Skills/Skill.h"

typedef struct {
	Skill* skill;
	DIRECTION dir;
} SkillParam;

class Background
{
private:
	BlockingQueue<SkillParam> skill_queue;
	std::thread umpires[10];

	// ��ų �ǰ����� ������
	void check_attack();
public:
	Background();

	// ��ų �ǰ����� ��û
	void cast_skill(Skill* skill, DIRECTION dir);
};