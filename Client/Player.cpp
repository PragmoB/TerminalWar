#include <cstdio>
#include <map>
#include <windows.h>
#include <mmsystem.h>

#include "interface.h"
#include "Graphic.h"
#include "Sound.h"
#include "Player.h"

#include "Items/Item.h"
#include "Items/Heart.h"
#include "Items/Energy.h"

#include "Skills/WindSlash.h"

extern Graphic graphic;
extern Sound sound;

std::map<DWORD, Player*> players;

Player::Player(COORD pos, int HP, char chracter, bool me, int len_skills, SKILL_TYPE skills[MAX_SKILL], int skill_levels[MAX_SKILL])
	: chracter(chracter), HP(HP), me(me), len_skills(len_skills)
{
	for (int i = 0; i < len_skills; i++)
		this->skills[i] = NULL;

	for (int i = 0; i < len_skills; i++)
		this->skills[i] = Skill::create_object_by_type(skills[i], this, skill_levels[i]);

	// �нú� ��ų �ɷ�ġ �ݿ�
	for (int i = 0; i < len_skills; i++)
	{
		PassiveSkill* passive_skill = dynamic_cast<PassiveSkill*>(this->skills[i]);
		if (passive_skill)
		{
			speed_increase_rate = (speed_increase_rate + 100) * (passive_skill->get_speed_rate() + 100) / 100 - 100;
			defense_rate = (defense_rate + 100) * (passive_skill->get_defense_rate() + 100) / 100 - 100;
			damage_increase_rate = (damage_increase_rate + 100) * (passive_skill->get_damage_rate() + 100) / 100 - 100;
			evasion_rate = (evasion_rate + 100) * (passive_skill->get_evasion_rate() + 100) / 100 - 100;
			mov_attack_damage += passive_skill->get_mov_attack_damage();
			mov_attack_range += passive_skill->get_mov_attack_range();
		}
	}
	// ȭ�鿡 ĳ���� ù ǥ��
	this->pos = graphic.get_client_pos_by_server_pos(pos);
	this->appear();
}

Skill* Player::get_skill(SKILL_TYPE skill_type)
{
	for (int i = 0; i < len_skills; i++)
	{
		if (skills[i]->get_type() == skill_type)
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
		graphic.draw(pos, chracter, GREEN, graphic.FIELD_BACKGROUND_COLOR);
	else
		graphic.draw(pos, chracter, WHITE, graphic.FIELD_BACKGROUND_COLOR);

	graphic.draw(pos_temp, "\u2665", DARK_RED, graphic.FIELD_BACKGROUND_COLOR); // ������ ��Ʈ
	pos_temp.X += 2;
	graphic.draw(pos_temp, HP_str, SKY_BLUE, graphic.FIELD_BACKGROUND_COLOR);
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
	
	graphic.draw(pos, ':', graphic.FIELD_BACKGROUND_COLOR, graphic.FIELD_BACKGROUND_COLOR);
	graphic.draw(pos_temp, ":::::::", graphic.FIELD_BACKGROUND_COLOR,  graphic.FIELD_BACKGROUND_COLOR);
}
void Player::move(DIRECTION dir)
{
	moving = true;

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

	moving = false;
}
void Player::move(COORD pos)
{
	// pos ��ȿ�� �˻�
	if (pos.X < 1 || FIELD_WIDTH < pos.X ||
		pos.Y < 1 || FIELD_HEIGHT < pos.Y)
		return;

	moving = true;
	this->disappear();
	this->pos = graphic.get_client_pos_by_server_pos(pos);
	this->appear();
	moving = false;
}

void Player::cast_skill(SKILL_TYPE skill_type, DIRECTION dir)
{
	ActiveSkill* skill = dynamic_cast<ActiveSkill*>(get_skill(skill_type));
	if (skill)
		graphic.cast_skill(skill, dir);
	return;
}

void Player::upgrade_skill(SKILL_TYPE before, SKILL_TYPE after)
{
	Skill* skill = NULL;

	for (int i = 0; i < len_skills; i++)
	{
		if (skills[i]->get_type() == before)
		{
			skill = skills[i];
			// �нú� ��ų�̸�
			PassiveSkill* passive_skill = dynamic_cast<PassiveSkill*>(skill);
			if (passive_skill)
			{
				// �÷��̾�� ����Ǿ� �ִ� �ش� ������ �нú� ��ų �ɷ�ġ �ʱ�ȭ
				speed_increase_rate = 100 * (speed_increase_rate + 100) / (passive_skill->get_speed_rate() + 100) - 100;
				defense_rate = 100 * (defense_rate + 100) / (passive_skill->get_defense_rate() + 100) - 100;
				damage_increase_rate = 100 * (damage_increase_rate + 100) / (passive_skill->get_damage_rate() + 100) - 100;
				evasion_rate = 100 * (evasion_rate + 100) / (passive_skill->get_evasion_rate() + 100) - 100;
				mov_attack_damage -= passive_skill->get_mov_attack_damage();
				mov_attack_range -= passive_skill->get_mov_attack_range();
			}

			// ��ȭ
			if (skills[i]->get_level() >= skills[i]->get_max_level())
			{
				delete skills[i];
				skill = skills[i] = Skill::create_object_by_type(after, this);
			}
			else
				skills[i]->level_up();

			break;
		}
	}
	// ���ο� ��ų
	if (!skill)
		skill = skills[len_skills++] = Skill::create_object_by_type(before, this);

	// �нú� ��ų�̸�
	PassiveSkill* passive_skill = dynamic_cast<PassiveSkill*>(skill);
	if (passive_skill)
	{
		// �÷��̾�� �ش� ������ �нú� ��ų �ɷ�ġ ����
		speed_increase_rate = (speed_increase_rate + 100) * (passive_skill->get_speed_rate() + 100) / 100 - 100;
		defense_rate = (defense_rate + 100) * (passive_skill->get_defense_rate() + 100) / 100 - 100;
		damage_increase_rate = (damage_increase_rate + 100) * (passive_skill->get_damage_rate() + 100) / 100 - 100;
		evasion_rate = (evasion_rate + 100) * (passive_skill->get_evasion_rate() + 100) / 100 - 100;
		mov_attack_damage += passive_skill->get_mov_attack_damage();
		mov_attack_range += passive_skill->get_mov_attack_range();
	}
}

void Player::attack(Player* player, SKILL_TYPE skill_type, bool evaded)
{
	Skill* skill = NULL;
	
	// ǳ������ �˱⿡ ���� ����
	if (skill_type == WIND)
		skill = &((WindSlash*)get_skill(WIND_SLASH))->wind; // '�˱�' ��ų�� ǳ���� �ȿ� �ֱ⿡ ���� ��������
	else
		skill = get_skill(skill_type);

	player->hit(dynamic_cast<ActiveSkill*>(skill), evaded);
}
void Player::hit(const ActiveSkill* skill, bool evaded)
{
	if (!skill)
		return;

	sound.request(HIT, skill->get_type());

	if (evaded)
		graphic.draw(pos, '*', BLUE, graphic.FIELD_BACKGROUND_COLOR);
	else
	{
		int damage = skill->get_damage();
		damage = damage * (skill->get_owner()->get_damage_increase_rate() + 100) * (100 - defense_rate) / 10000;
		HP -= damage;
		graphic.draw(pos, '*', RED, graphic.FIELD_BACKGROUND_COLOR);
	}
}
void Player::hit_mov_attack(const Player* attacker, bool evaded)
{
	if (!attacker)
		return;

	if (evaded)
		graphic.draw(pos, '*', BLUE, graphic.FIELD_BACKGROUND_COLOR);
	else
	{
		int damage = attacker->get_mov_attack_damage();
		damage = damage * (attacker->get_damage_increase_rate() + 100) * (100 - defense_rate) / 10000;
		HP -= damage;
		graphic.draw(pos, '*', RED, graphic.FIELD_BACKGROUND_COLOR);
	}
}

void Player::earn_item(Item* item)
{
	if (!item)
		return;

	switch (item->get_type())
	{
	case HEART:
	{
		const Heart* heart_item = dynamic_cast<Heart*>(item);
		HP += heart_item->get_amount();
		break;
	}
	}

	sound.request(EARN_ITEM, item->get_type());

	item->disappear();
	delete item;
}

bool Player::is_me()
{
	return me;
}
bool Player::is_moving()
{
	return moving;
}

int Player::get_damage_increase_rate() const
{
	return damage_increase_rate;
}
COORD Player::get_pos() const
{
	return pos;
}
int Player::get_mov_attack_damage() const
{
	return mov_attack_damage;
}
Player::~Player()
{
	this->disappear();
	for (int i = 0; i < len_skills; i++)
		delete skills[i];
}