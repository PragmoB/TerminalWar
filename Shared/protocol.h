#pragma once

#include <windows.h>

#include "interface.h"

const int PORT = 44400;

enum DIRECTION
{	UP = 1, DOWN, LEFT, RIGHT };
enum PDU_TYPE
{	HELLO = 1, MOV, CAST_SKILL, HIT, DIE	};
enum SKILL_TYPE
{
	SHOOT = 1,
	SLASH, LIGHTSABER_SLASH, ZWEIHANDER_SLASH, WIND_SLASH, WIND
};

// ��������, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = HELLO;
	DWORD id;
	int HP;
	char chracter;
	COORD pos;

	int len_skills;
	SKILL_TYPE skills[MAX_ACTIVE_SKILL + 1];
	int skill_levels[MAX_ACTIVE_SKILL + 1];
} PDUHello;
// ĳ���� ����, Ŭ�� <=> ����
typedef struct
{
	const PDU_TYPE type = MOV;
	DWORD id;
	DIRECTION dir;
} PDUMov;
// ��ų ���, Ŭ�� <=> ����
typedef struct
{
	const PDU_TYPE type = CAST_SKILL;
	SKILL_TYPE skill_type;
	DWORD id;
	DIRECTION dir;
} PDUCastSkill;
// ��ų ����, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = HIT;
	DWORD attacker_id;
	DWORD victim_id;
	SKILL_TYPE skill_type;
} PDUHit;
// ���� ������ �Ǵ� ����, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = DIE;
	DWORD id;
} PDUDie;