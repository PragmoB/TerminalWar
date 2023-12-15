#include <cstring>

#include "Skills/Shoot.h"
#include "Player.h"
#include "Graphic.h"
#include "interface.h"

extern Graphic graphic;

Shoot::Shoot(Player* owner, int level)
	: Skill(owner, level)
{
}

/* �Ѿ� ��ġ�� �����ϰ�, graphic�� �ݿ��� */

bool Shoot::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	Player* owner = get_owner();
	COORD pos = owner->get_pos();

	const int bps = get_bps();
	int remain_distance = get_distance();
	switch (dir)
	{
	case UP:
		// ���� ��ų� �����Ÿ��� 0�� �� ������ ��� ���ư�
		for (; 0 < remain_distance && FIELD.Top < pos.Y - 1; remain_distance--)
		{
			pos.Y--; // �Ѿ��� ���� �̵�
			graphic.draw(pos, '|'); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps); // �߻� �ӵ� ����
			graphic.draw(pos, ' '); // �ܻ� �����
		}
		break;
	case DOWN:
		for (; 0 < remain_distance && pos.Y < FIELD.Bottom; remain_distance--)
		{
			pos.Y++;
			graphic.draw(pos, '|'); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' '); // �ܻ� �����
		}
		break;
	case LEFT:
		for (; 0 < remain_distance && FIELD.Left < pos.X - 2; remain_distance--)
		{
			pos.X -= 2;
			graphic.draw(pos, "--"); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, "  "); // �ܻ� �����
		}
		break;
	case RIGHT:
		pos.X--;
		for (; 0 < remain_distance && pos.X + 2 < FIELD.Left + 2 * FIELD_WIDTH; remain_distance--)
		{
			pos.X += 2;
			graphic.draw(pos, "--"); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, "  "); // �ܻ� �����
		}
		break;
	}

	return true;
}

int Shoot::get_damage() const
{
	return DAMAGE[get_level() - 1];
}
int Shoot::get_cooldown() const
{
	return COOLDOWN[get_level() - 1];
}
int Shoot::get_bps() const
{
	return BPS[get_level() - 1];
}
int Shoot::get_distance() const
{
	return DISTANCE[get_level() - 1];
}
SKILL_TYPE Shoot::get_type() const
{
	return SHOOT;
}
int Shoot::get_max_level() const
{
	return MAX_LEVEL;
}
const char* Shoot::get_skill_name() const
{
	return "���ڹ���";
}
void Shoot::get_level_up_message(char* output, int len) const
{
	const char* skill_name = Shoot::get_skill_name();
	const int LEVEL = get_level();

	if (LEVEL == MAX_LEVEL)
		sprintf_s(output, len, "[��Ƽ��] %s ��ȭ", skill_name);
	else
	{
		sprintf_s(output, len, "[��Ƽ��] %-16s : ���ݷ� %d%% ���� | ��Ÿ�� %d%% ���� | ź�� %d%% ���� | ��Ÿ� %dĭ ����",
			skill_name,
			100 * (DAMAGE[LEVEL] - DAMAGE[LEVEL - 1]) / DAMAGE[LEVEL - 1],
			100 * (COOLDOWN[LEVEL - 1] - COOLDOWN[LEVEL]) / COOLDOWN[LEVEL - 1],
			100 * (BPS[LEVEL] - BPS[LEVEL - 1]) / BPS[LEVEL - 1],
			DISTANCE[LEVEL] - DISTANCE[LEVEL - 1]);
	}
}
void Shoot::get_learn_message(char* output, int len) const
{
	const char* skill_name = Shoot::get_skill_name();

	sprintf_s(output, len, "[��Ƽ��] %-16s : ����� ������ ��� �������� ���ڸ� �߻��մϴ�. | ���ݷ� %d | ��Ÿ�� %d.%d�� | ź�� %dbps | ��Ÿ� %dĭ",
		skill_name,
		DAMAGE[0],
		COOLDOWN[0] / 1000, COOLDOWN[0] % 1000,
		BPS[0],
		DISTANCE[0]);
}
int Shoot::get_ordinal() const
{
	return 1;
}
bool Shoot::upgradable() const
{
	return true;
}
bool Shoot::upgradable_to(SKILL_TYPE type) const
{
	switch (type)
	{
	case SNIPE:
		return true;
	default:
		return false;
	}
}