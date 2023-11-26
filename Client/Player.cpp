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

#include "Skills/Skill.h"
#include "Skills/Shoot.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

extern Graphic graphic;
extern Sound sound;

std::map<DWORD, Player*> players;

Player::Player(COORD pos, int HP, char chracter, bool me, int len_active_skills, SKILL_TYPE skills[MAX_ACTIVE_SKILL], int skill_levels[MAX_ACTIVE_SKILL])
	: chracter(chracter), HP(HP), me(me), len_active_skills(len_active_skills)
{
	for (int i = 0; i < len_active_skills; i++)
		this->active_skills[i] = NULL;

	for (int i = 0; i < len_active_skills; i++)
		this->active_skills[i] = Skill::create_object_by_type(skills[i], this, skill_levels[i]);

	// 화면에 캐릭터 첫 표시
	this->pos = graphic.get_client_pos_by_server_pos(pos);
	this->appear();
}

Skill* Player::get_active_skill(SKILL_TYPE skill_type)
{
	for (int i = 0; i < len_active_skills; i++)
	{
		if (active_skills[i]->get_type() == skill_type)
			return active_skills[i];
	}
	return NULL;
}
void Player::appear()
{
	/* 체력 상태 표시줄이 나올 위치 계산 */

	// 기본적으로 체력 상태 표시줄은 캐릭터 1칸 밑 가운데 정렬하여 표시
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < FIELD.Left + 1) // 체력 상태 표시줄이 경기장 왼쪽 끝을 삐져나온다면
		pos_temp.X = FIELD.Left + 1; // 경기장 왼쪽 끝으로 맞춤
	else if ((signed)pos_temp.X > FIELD.Left + 2 * FIELD_WIDTH - 7) // 경기장 오른쪽 끝을 삐져나온다면
		pos_temp.X = FIELD.Left + 2 * FIELD_WIDTH - 7; // 경기장 오른쪽 끝으로 맞춤

	/* 체력 숫자 => 5자리 문자열로 변환 */

	char HP_str[6] = "";
	sprintf_s(HP_str, "%05d", HP);

	/* 출력 */

	if (me)
		graphic.draw(pos, chracter, GREEN);
	else
		graphic.draw(pos, chracter);

	graphic.draw(pos_temp, "\u2665", DARK_RED); // 빨간색 하트
	pos_temp.X += 2;
	graphic.draw(pos_temp, HP_str, SKY_BLUE);
}
void Player::disappear()
{
	/* 체력 상태 표시줄이 있는 위치 계산 */

	// 기본적으로 체력 상태 표시줄은 캐릭터 1칸 밑 가운데 정렬하여 표시
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < FIELD.Left + 1) // 체력 상태 표시줄이 경기장 왼쪽 끝을 삐져나온다면
		pos_temp.X = FIELD.Left + 1; // 경기장 왼쪽 끝으로 맞춤
	else if ((signed)pos_temp.X > FIELD.Left + 2 * FIELD_WIDTH - 7) // 경기장 오른쪽 끝을 삐져나온다면
		pos_temp.X = FIELD.Left + 2 * FIELD_WIDTH - 7; // 경기장 오른쪽 끝으로 맞춤

	/* 지우기 */
	
	graphic.draw(pos, ' ');
	graphic.draw(pos_temp, "       ");
}
void Player::move(DIRECTION dir)
{
	moving = true;

	this->disappear();

	switch (dir)
	{
	case UP: pos.Y--; break;
	case DOWN: pos.Y++; break;

	// 가로방향 이동은 콘솔창의 커서 사각형의 가로 세로 길이 비율이 2:1임을 고려하여 움직임
	case LEFT: pos.X -= 2; break;
	case RIGHT: pos.X += 2; break;
	}
	this->appear();

	moving = false;
}

void Player::cast_skill(SKILL_TYPE skill_type, DIRECTION dir)
{
	Skill* skill = get_active_skill(skill_type);
	if (skill)
		graphic.cast_skill(skill, dir);
	return;
}

void Player::upgrade_skill(SKILL_TYPE before, SKILL_TYPE after)
{
	for (int i = 0; i < len_active_skills; i++)
	{
		if (active_skills[i]->get_type() == before)
		{
			// 진화
			if (active_skills[i]->get_level() >= active_skills[i]->get_max_level())
			{
				delete active_skills[i];
				active_skills[i] = Skill::create_object_by_type(after, this);
			}
			else
				active_skills[i]->level_up();

			return;
		}
	}
	
	// 새로운 스킬
	active_skills[len_active_skills++] = Skill::create_object_by_type(before, this);
}

void Player::attack(Player* player, SKILL_TYPE skill_type)
{
	Skill* skill = NULL;
	
	// 풍마참의 검기에 맞은 경우는
	if (skill_type == WIND)
		skill = &((WindSlash*)get_active_skill(WIND_SLASH))->wind; // '검기' 스킬이 풍마참 안에 있기에 따로 꺼내야함
	else
		skill = get_active_skill(skill_type);

	player->hit(skill);
}
void Player::hit(const Skill* skill)
{
	if (!skill)
		return;

	sound.request(HIT, skill->get_type());

	HP -= skill->get_damage();
	graphic.draw(pos, '*', RED);
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

COORD Player::get_pos() const
{
	return pos;
}

Player::~Player()
{
	this->disappear();
	for (int i = 0; i < len_active_skills; i++)
		delete active_skills[i];
}