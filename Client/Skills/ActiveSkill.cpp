#include "Skill.h"
#include "ActiveSkill.h"

#include "Sound.h"

extern Sound sound;

ActiveSkill::ActiveSkill(Player* owner, int level) : Skill(owner, level)
{

}

bool ActiveSkill::cast(DIRECTION dir)
{
	sound.request(CAST_SKILL, get_type());
	return true; // ¼º°ø
}