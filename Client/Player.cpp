#include <cstdio>
#include <windows.h>
#include <mmsystem.h>

#include "interface.h"
#include "Graphic.h"
#include "Sound.h"
#include "Player.h"

#include "Skills/Shoot.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

extern Graphic graphic;
extern Sound sound;

Player::Player(COORD pos, int HP, char chracter, bool me, int len_skills, SKILL_TYPE skills[MAX_ACTIVE_SKILL])
	: chracter(chracter), HP(HP), me(me), len_skills(len_skills)
{
	for (int i = 0; i < len_skills; i++)
	{
		switch (skills[i])
		{
		case SHOOT:			   this->skills[i] = new Shoot(this); break;
		case SLASH:			   this->skills[i] = new Slash(this); break;
		case LIGHTSABER_SLASH: this->skills[i] = new LightsaberSlash(this); break;
		case ZWEIHANDER_SLASH: this->skills[i] = new ZweihanderSlash(this); break;
		case WIND_SLASH:	   this->skills[i] = new WindSlash(this); break;
		}
	}

	// ȭ�鿡 ĳ���� ù ǥ��
	pos.X *= 2; // ���ι��� �̵��� 2ĭ������ �����
	pos.X += FIELD.Left - 1;
	pos.Y += FIELD.Top;
	this->pos = pos;
	this->appear();
}

Skill* Player::get_skill(SKILL_TYPE skill_type)
{
	for (int i = 0; i < len_skills; i++)
	{
		if (skills[i]->type == skill_type)
			return skills[i];
	}
	return NULL;
}
void Player::appear()
{
	/* ü�� ���� ǥ������ ���� ��ġ ��� */

	// �⺻������ ü�� ���� ǥ������ ĳ���� 1ĭ �� ��� �����Ͽ� ǥ��
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < FIELD.Left + 1) // ü�� ���� ǥ������ ����� ���� ���� �������´ٸ�
		pos_temp.X = FIELD.Left + 1; // ����� ���� ������ ����
	else if ((signed)pos_temp.X > FIELD.Left + 2 * FIELD_WIDTH - 7) // ����� ������ ���� �������´ٸ�
		pos_temp.X = FIELD.Left + 2 * FIELD_WIDTH - 7; // ����� ������ ������ ����

	/* ü�� ���� => 5�ڸ� ���ڿ��� ��ȯ */

	char HP_str[6] = "";
	sprintf_s(HP_str, "%05d", HP);

	/* ��� */

	if (me)
		graphic.draw(pos, chracter, GREEN);
	else
		graphic.draw(pos, chracter);

	graphic.draw(pos_temp, "\u2665", DARK_RED); // ������ ��Ʈ
	pos_temp.X += 2;
	graphic.draw(pos_temp, HP_str, SKY_BLUE);
}
void Player::disappear()
{
	/* ü�� ���� ǥ������ �ִ� ��ġ ��� */

	// �⺻������ ü�� ���� ǥ������ ĳ���� 1ĭ �� ��� �����Ͽ� ǥ��
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < FIELD.Left + 1) // ü�� ���� ǥ������ ����� ���� ���� �������´ٸ�
		pos_temp.X = FIELD.Left + 1; // ����� ���� ������ ����
	else if ((signed)pos_temp.X > FIELD.Left + 2 * FIELD_WIDTH - 7) // ����� ������ ���� �������´ٸ�
		pos_temp.X = FIELD.Left + 2 * FIELD_WIDTH - 7; // ����� ������ ������ ����

	/* ����� */
	
	graphic.draw(pos, ' ');
	graphic.draw(pos_temp, "       ");
}
void Player::move(DIRECTION dir)
{
	this->disappear();

	switch (dir)
	{
	case UP: pos.Y--; break;
	case DOWN: pos.Y++; break;

	// ���ι��� �̵��� �ܼ�â�� Ŀ�� �簢���� ���� ���� ���� ������ 2:1���� ����Ͽ� ������
	case LEFT: pos.X -= 2; break;
	case RIGHT: pos.X += 2; break;
	}
	this->appear();
}

void Player::cast_skill(SKILL_TYPE skill_type, DIRECTION dir)
{
	Skill* skill = get_skill(skill_type);
	if (skill)
		graphic.cast_skill(skill, dir);
	return;
}

void Player::upgrade_skill(SKILL_TYPE before, SKILL_TYPE after)
{
	for (int i = 0; i < len_skills; i++)
	{
		if (skills[i]->type == before)
		{
			skills[i]->level_up();

			if (skills[i]->get_level() >= skills[i]->MAX_LEVEL)
			{/*
				switch (after)
				{
				case LI
				}*/

			}
		}
	}
}

void Player::attack(Player* player, SKILL_TYPE skill_type)
{
	Skill* skill;
	
	// ǳ������ �˱⿡ ���� ����
	if (skill_type == WIND)
		skill = &((WindSlash*)get_skill(WIND_SLASH))->wind; // '�˱�' ��ų�� ǳ���� �ȿ� �ֱ⿡ ���� ��������
	else
		skill = get_skill(skill_type);

	if (skill)
		skill->attack(player);
}
void Player::hit(const Skill* skill)
{
	HP -= skill->get_damage();
	graphic.draw(pos, '*', RED);
}

bool Player::is_me()
{
	return me;
}

Player::~Player()
{
	this->disappear();
}

COORD Player::get_pos() const
{
	return pos;
}