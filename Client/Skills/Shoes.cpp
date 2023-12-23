#include <iostream>

#include "Shoes.h"

Shoes::Shoes(Player* owner, int level) : PassiveSkill(owner, level)
{

}
int Shoes::get_speed_rate()
{
	return SPEED_RATE[get_level() - 1];
}

SKILL_TYPE Shoes::get_type() const
{
	return SHOES;
}
int Shoes::get_max_level() const
{
	return MAX_LEVEL;
}
int Shoes::get_ordinal() const
{
	return 1;
}

bool Shoes::upgradable() const
{
	return false;
}
bool Shoes::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool Shoes::downgradable() const
{
	return false;
}
bool Shoes::downgradable_to(SKILL_TYPE type) const
{
	return false;
}

const char* Shoes::get_skill_name() const
{
	return "�Ź�";
}
void Shoes::get_level_up_message(char* output, int len) const
{
	const char* skill_name = Shoes::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[�нú�] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[�нú�] %-16s : �̵��ӵ� %d%% ����",
			skill_name, 100 * (SPEED_RATE[LEVEL] + 100) / (SPEED_RATE[LEVEL - 1] + 100) - 100);
	}
}
void Shoes::get_learn_message(char* output, int len) const
{
	const char* skill_name = Shoes::get_skill_name();

	sprintf_s(output, len, "[�нú�] %-16s : �� ������ �̵��մϴ�. | �̵��ӵ� %d%% ����",
		skill_name,
		SPEED_RATE[0]);
}