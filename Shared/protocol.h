#pragma once

#include <windows.h>

#include "Items/Item.h"
#include "Skills/Skill.h"

#include "interface.h"

const int PORT = 44400;

enum PDU_TYPE
{	UDP_HELLO = 1, HELLO, ITEM_INFO, EARN_ITEM, MOV, CAST_SKILL, HIT, UPGRADE_SKILL_OPTION_INFO, UPGRADE_SKILL, DIE	};

// Ŭ���̾�Ʈ �� UDP ��Ʈ �˸�, Ŭ�� => ����
typedef struct
{
	const PDU_TYPE type = UDP_HELLO;
	DWORD id;
} PDUUDPHello;

// ��������, Ŭ�� <=> ����
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
// ������ ���� �˸�, Ŭ�� <= ����
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
// ������ ȹ�� �˸�, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = EARN_ITEM;
	DWORD id;
	COORD pos;
} PDUEarnItem;
// ĳ���� ����, Ŭ�� => ����
typedef struct
{
	const PDU_TYPE type = MOV;
	DWORD id;
	DIRECTION dir;
} PDUMovReq;
// ĳ���� ����, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = MOV;
	DWORD id;
	COORD pos;
} PDUMovRes;
// ��ų ���, Ŭ�� <=> ����
typedef struct
{
	const PDU_TYPE type = CAST_SKILL;
	DWORD id;
	SKILL_TYPE skill_type;
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
// ��ų ��ȭ ������ ���� �˸�, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = UPGRADE_SKILL_OPTION_INFO;
	union
	{
		int skill_types[NUM_UPGRADE_SKILL_OPTIONS];
		SKILL_TYPE active_skill_types[NUM_UPGRADE_SKILL_OPTIONS];
	};
	bool skill_is_active[NUM_UPGRADE_SKILL_OPTIONS];
} PDUUpgradeSkillOptionInfo;
// ��ų ��ȭ ��û, Ŭ�� <=> ����
typedef struct
{
	const PDU_TYPE type = UPGRADE_SKILL;
	DWORD id;
	union
	{
		SKILL_TYPE active_skill_type;
	};
	union
	{
		SKILL_TYPE upgraded_active_skill_type;
	};
	bool skill_is_active;
	
} PDUUpgradeSkill;
// ���� ������ �Ǵ� ����, Ŭ�� <= ����
typedef struct
{
	const PDU_TYPE type = DIE;
	DWORD id;
	int level;
	DWORD attacker_id = NULL;
} PDUDie;