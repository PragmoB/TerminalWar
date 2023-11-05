#include "protocol.h"
#include "Player.h"
#include "Sound.h"
#include "Skills/Skill.h"

#include <ctime>

extern Sound sound;

Skill::Skill(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: owner(owner), level(level), type(type), MAX_LEVEL(MAX_LEVEL)
{

}

bool Skill::cast(DIRECTION dir)
{
	clock_t now = clock();

	// ���� ���ð� �˻�
	if (now < next_able_time)
		return false; // ����

	sound.request(CAST_SKILL, type);
	next_able_time = now + cooldown;
	return true; // ����
}
void Skill::attack(Player* player)
{
	if (player)
		player->hit(this);
}
void Skill::level_up()
{
	level++;
}
int Skill::get_level() const
{
	return level;
}
int Skill::get_damage() const
{
	return damage;
}
int Skill::get_cooldown() const
{
	return cooldown;
}