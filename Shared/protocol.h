#pragma once

#define _WINSOCKAPI_
#include <windows.h>

#include "Items/Item.h"
#include "Skills/Skill.h"
#include "Values/player_values.h"

#include "Direction.h"

const int PORT = 44400;

enum PDU_TYPE
{	UDP_HELLO = 1, HELLO, ITEM_INFO, EARN_ITEM, MOV, CAST_SKILL, HIT, HIT_MOV_ATTACK,
	UPGRADE_SKILL_OPTION_INFO, UPGRADE_SKILL, DIE	};

// 클라이언트 측 UDP 포트 알림, 클라 => 서버
typedef struct
{
	const PDU_TYPE type = UDP_HELLO;
	DWORD id;
} PDUUDPHello;

// 유저접속, 클라 <=> 서버
typedef struct
{
	const PDU_TYPE type = HELLO;
	DWORD id;
	int HP;
	char chracter;
	COORD pos;

	int len_skills;
	SKILL_TYPE skills[MAX_SKILL + 1];
	int skill_levels[MAX_SKILL + 1];
} PDUHello;
// 아이템 존재 알림, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = ITEM_INFO;
	ITEM_TYPE item_type;
	COORD pos;
	union
	{
		int level;
	};
} PDUItemInfo;
// 아이템 획득 알림, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = EARN_ITEM;
	DWORD id;
	COORD pos;
} PDUEarnItem;
// 캐릭터 무빙, 클라 => 서버
typedef struct
{
	const PDU_TYPE type = MOV;
	DWORD id;
	DIRECTION dir;
} PDUMovReq;
// 캐릭터 무빙, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = MOV;
	DWORD id;
	COORD pos;
} PDUMovRes;
// 스킬 사용, 클라 <=> 서버
typedef struct
{
	const PDU_TYPE type = CAST_SKILL;
	DWORD id;
	SKILL_TYPE skill_type;
	DIRECTION dir;
} PDUCastSkill;
// 스킬 명중, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = HIT;
	DWORD attacker_id;
	DWORD victim_id;
	SKILL_TYPE skill_type;
	bool evaded;
} PDUHit;
typedef struct
{
	const PDU_TYPE type = HIT_MOV_ATTACK;
	DWORD attacker_id;
	DWORD victim_id;
	bool evaded;
} PDUHitMovAttack;
// 스킬 강화 선택지 정보 알림, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = UPGRADE_SKILL_OPTION_INFO;
	SKILL_TYPE skill_types[NUM_UPGRADE_SKILL_OPTIONS];
} PDUUpgradeSkillOptionInfo;
// 스킬 강화 요청, 클라 <=> 서버
typedef struct
{
	const PDU_TYPE type = UPGRADE_SKILL;
	DWORD id;
	SKILL_TYPE skill_type;
	SKILL_TYPE upgraded_skill_type;
} PDUUpgradeSkill;
// 접속 끊어짐 또는 죽음, 클라 <= 서버
typedef struct
{
	const PDU_TYPE type = DIE;
	DWORD id;
	int level;
	DWORD attacker_id = NULL;
} PDUDie;