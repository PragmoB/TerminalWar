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

	// 스킬 피격판정 스레드
	void check_attack();
public:
	Background();

	// 스킬 피격판정 요청
	void cast_skill(Skill* skill, DIRECTION dir);
};