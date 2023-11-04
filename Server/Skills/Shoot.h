#pragma once

#include <iostream>
#include <windows.h>

#include "protocol.h"
#include "Skills/Skill.h"

class Shoot : public Skill
{
private:
	static const int DAMAGE[5]; // [i] = (i + 1)레벨에서 총알의 데미지
	static const int COOLDOWN[5]; // [i] = (i + 1)레벨에서 재사용 대기시간
	static const int BPS[5]; // [i] = (i + 1)레벨에서 총알의 속도
	static const int DISTANCE[5]; // [i] = (i + 1)레벨에서 날아갈 거리
protected:
	int bps; // the valocity of the bullet. blocks per second
	int distance; // 날아갈 거리
public:
	Shoot(Client* owner, int level = 1, SKILL_TYPE type = SHOOT, int MAX_LEVEL = 5);
	virtual bool cast(DIRECTION dir);
	virtual void level_up();

	int get_bps();
	int get_distance();
};