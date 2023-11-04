#pragma once

#include <windows.h>

#include "protocol.h"
#include "Skills/Slash.h"

class LightsaberSlash : public Slash
{
private:
	static const int DAMAGE[10];
	static const int COOLDOWN[10];
public:
	LightsaberSlash(const Player* owner, int level = 1, SKILL_TYPE type = LIGHTSABER_SLASH, int MAX_LEVEL = 10);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();
};