#pragma once

#include <windows.h>

#include "interface.h"
#include "Skills/Skill.h"

class Wind : public Skill
{
private:
	static const int DAMAGE[10];
	static const int COOLDOWN[10];
	
public:
	Wind(const Player* owner, int level = 1, SKILL_TYPE type = WIND, int MAX_LEVEL = 10);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();
};