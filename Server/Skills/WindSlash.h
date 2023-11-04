#pragma once

#include <windows.h>

#include "interface.h"
#include "Skills/Slash.h"
#include "Skills/Wind.h"

class WindSlash : public Slash
{
private:
	static const int DAMAGE[10];
	static const int COOLDOWN[10];
public:
	Wind wind;
	WindSlash(const Client* owner, int level = 1, SKILL_TYPE type = WIND_SLASH, int MAX_LEVEL = 10);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();
};