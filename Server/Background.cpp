#include <thread>

#include "Background.h"
#include "Skills/LightsaberSlash.h"

Background background;

Background::Background() : skill_queue(3)
{
	// 피격판정 스레드 풀 생성
	for (int i = 0; i < 10; i++)
		umpires[i] = std::thread(&Background::check_attack, this);
}
void Background::check_attack()
{
	while (1)
	{
		SkillParam skill_param = skill_queue.Dequeue();
		skill_param.skill->cast(skill_param.dir);
	}
}

void Background::cast_skill(Skill* skill, DIRECTION dir)
{
	skill_queue.Enqueue(SkillParam{ skill, dir });
}