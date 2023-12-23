#include "PassiveSkill.h"

PassiveSkill::PassiveSkill(Player* owner, int level) : Skill(owner, level)
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