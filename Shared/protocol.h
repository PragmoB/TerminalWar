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

// 유저접속, 클라 <= 서버
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
// 캐릭터 무빙, 클라 <=> 서버
typedef struct
{
	const PDU_TYPE type = MOV;
	DWORD id;
	DIRECTION dir;
} PDUMov;
// 스킬 사용, 클라 <=> 서버
typedef struct
{
	const PDU_TYPE type = CAST_SKILL;
	SKILL_TYPE skill_type;
	DWORD id;
	DIRECTION dir;
} PDUCastSkill;
// 스킬 명중, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = HIT;
	DWORD attacker_id;
	DWORD victim_id;
	SKILL_TYPE skill_type;
} PDUHit;
// 접속 끊어짐 또는 죽음, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = DIE;
	DWORD id;
} PDUDie;