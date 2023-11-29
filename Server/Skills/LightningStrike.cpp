#include "Background.h"
#include "Client.h"

#include "LightningStrike.h"

extern Background background;

LightningStrike::LightningStrike(Client* owner, int level)
	: Skill(owner, level)
{
}

bool LightningStrike::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	// ���� ���� �� �������� �ǰ� ����
	const int RANGE = 2;

	Client* owner = get_owner();
	const COORD pos = owner->get_pos();

	// ��ų ��� �� Ŭ���̾�Ʈ�� ��ǥ
	COORD pos_temp = pos;

	owner->bind(1000);
	Sleep(350);

	for (int i = 0; i < 19; i++)
		owner->move(dir, true);

	switch (dir)
	{
	case UP:	pos_temp.Y -= 19; break;
	case DOWN:  pos_temp.Y += 19; break;
	case LEFT:  pos_temp.X -= 19; break;
	case RIGHT: pos_temp.X -= 19; break;
	default: return false;
	}

	if ((signed)pos_temp.X < 1)			   pos_temp.X = 1;
	if ((signed)pos_temp.Y < 1)			   pos_temp.Y = 1;
	if ((signed)pos_temp.X > FIELD_WIDTH)  pos_temp.X = FIELD_WIDTH;
	if ((signed)pos_temp.Y > FIELD_HEIGHT) pos_temp.Y = FIELD_HEIGHT;


	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
	{
		Client* victim = (*iter);
		COORD victim_pos = (*iter)->get_pos();

		switch (dir)
		{
		case UP:
			if (pos_temp.Y <= victim_pos.Y && victim_pos.Y <= pos.Y &&
				(signed)pos.X - RANGE <= victim_pos.X && victim_pos.X <= (signed)pos.X + RANGE)
				victim->hit(this);
			break;
		case DOWN:
			if (pos.Y <= victim_pos.Y && victim_pos.Y <= pos_temp.Y &&
				(signed)pos.X - RANGE <= victim_pos.X && victim_pos.X <= (signed)pos.X + RANGE)
				victim->hit(this);
			break;
		case LEFT:
			if (pos_temp.X <= victim_pos.X && victim_pos.X <= pos.X &&
				(signed)pos.Y - RANGE <= victim_pos.Y && victim_pos.Y <= (signed)pos.Y + RANGE)
				victim->hit(this);
			break;
		case RIGHT:
			if (pos.X <= victim_pos.X && victim_pos.X <= pos_temp.X &&
				(signed)pos.Y - RANGE <= victim_pos.Y && victim_pos.Y <= (signed)pos.Y + RANGE)
				victim->hit(this);
			break;
		}
	}
	return true;
}


int LightningStrike::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int LightningStrike::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
SKILL_TYPE LightningStrike::get_type() const
{
	return LIGHTNING_STRIKE;
}
int LightningStrike::get_max_level() const
{
	return MAX_LEVEL;
}
int LightningStrike::get_ordinal() const
{
	return 1;
}
bool LightningStrike::upgradable() const
{
	return false;
}
bool LightningStrike::upgradable_to(SKILL_TYPE type) const
{
	return false;
}
bool LightningStrike::downgradable() const
{
	return false;
}
bool LightningStrike::downgradable_to(SKILL_TYPE type) const
{
	return false;
}