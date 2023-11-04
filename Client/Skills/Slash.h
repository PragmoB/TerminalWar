#pragma once

#include <windows.h>

#include "interface.h"
#include "Skills/Skill.h"

class Slash : public Skill
{
private:
	static const int DAMAGE[5];
	static const int COOLDOWN[5];
public:
	Slash(const Player* owner, int level = 1, SKILL_TYPE type = SLASH, int MAX_LEVEL = 5);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();
};