#include "Skills/Shoot.h"
#include "Player.h"
#include "Graphic.h"
#include "interface.h"

extern Graphic graphic;

const int Shoot::DAMAGE[] = { 100, 105, 110, 115, 120 };
const int Shoot::COOLDOWN[] = { 1200, 1080, 972, 875, 788 };
const int Shoot::BPS[] = { 25, 26, 27, 28, 29 };
const int Shoot::DISTANCE[] = { 10, 11, 12, 13, 14 };

Shoot::Shoot(const Player* owner, int level, SKILL_TYPE type, int MAX_LEVEL)
	: Skill(owner, level, type, MAX_LEVEL)
{
	damage = DAMAGE[level - 1];
	cooldown = COOLDOWN[level - 1];
	bps = BPS[level - 1];
	distance = DISTANCE[level - 1];
}

/* �Ѿ� ��ġ�� �����ϰ�, graphic�� �ݿ��� */

bool Shoot::cast(DIRECTION dir)
{
	if (!Skill::cast(dir))
		return false;

	COORD pos = owner->get_pos();

	const char shape_horizontal = '-', shape_vertical = '|';
	int remain_distance = distance;
	switch (dir)
	{
	case UP:
		// ���� ��ų� �����Ÿ��� 0�� �� ������ ��� ���ư�
		for (; 0 < remain_distance && FIELD.Top < pos.Y - 1; remain_distance--)
		{
			pos.Y--; // �Ѿ��� ���� �̵�
			graphic.draw(pos, shape_vertical, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps); // �߻� �ӵ� ����
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	case DOWN:
		for (; 0 < remain_distance && pos.Y < FIELD.Bottom; remain_distance--)
		{
			pos.Y++;
			graphic.draw(pos, shape_vertical, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	case LEFT:
		for (; 0 < remain_distance && FIELD.Left < pos.X - 2; remain_distance--)
		{
			pos.X -= 2;
			graphic.draw(pos, shape_horizontal, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	case RIGHT:
		for (; 0 < remain_distance && pos.X + 2 < FIELD.Left + 2 * FIELD_WIDTH; remain_distance--)
		{
			pos.X += 2;
			graphic.draw(pos, shape_horizontal, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	}

	return true;
}
void Shoot::level_up()
{
	if (get_level() < MAX_LEVEL)
		Skill::level_up();

	damage = DAMAGE[get_level() - 1];
	cooldown = COOLDOWN[get_level() - 1];
	bps = BPS[get_level() - 1];
	distance = DISTANCE[get_level() - 1];
}

int Shoot::get_bps()
{
	return bps;
}
int Shoot::get_distance()
{
	return distance;
}