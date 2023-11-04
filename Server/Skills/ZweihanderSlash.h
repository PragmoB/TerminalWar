#pragma once

#include <windows.h>

#include "protocol.h"
#include "Skills/Slash.h"

class ZweihanderSlash : public Slash
{
private:
	static const int DAMAGE[10];
	static const int COOLDOWN[10];
public:
	ZweihanderSlash(Client* owner, int level = 1, SKILL_TYPE type = ZWEIHANDER_SLASH, int MAX_LEVEL = 10);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();
};