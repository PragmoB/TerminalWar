#include "PassiveSkill.h"

PassiveSkill::PassiveSkill(Client* owner, int level) : Skill(owner, level)
{

}

int PassiveSkill::get_speed_rate()
{
	return 0;
}
int PassiveSkill::get_defense_rate()
{
	return 0;
}
int PassiveSkill::get_damage_rate()
{
	return 0;
}
int PassiveSkill::get_evasion_rate()
{
	return 0;
}
int PassiveSkill::get_mov_attack_damage()
{
	return 0;
}
int PassiveSkill::get_mov_attack_range()
{
	return 0;
}