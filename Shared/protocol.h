#pragma once

const int PORT = 44400;

enum DIRECTION
{	UP = 1, DOWN, LEFT, RIGHT };
enum TYPE_PDU
{	HELLO, MOV, SHOOT, HIT, DIE	};

// ��������, Ŭ�� <= ����
typedef struct PDUHello
{
	const BYTE type = HELLO;
	DWORD id;
	int HP;
	char chracter;
	COORD pos;
} PDUHello;
// ĳ���� ����, Ŭ�� <=> ����
typedef struct PDUMov
{
	const BYTE type = MOV;
	DWORD id;
	DIRECTION dir;
} PDUMov;
// �Ѿ� �߻�, Ŭ�� <=> ����
typedef struct PDUShoot
{
	const BYTE type = SHOOT;
	DWORD id;
	DIRECTION dir;
} PDUShoot;
// �Ѿ� ����, Ŭ�� <= ����
typedef struct PDUHit
{
	const BYTE type = HIT;
	DWORD id;
	int damage;
} PDUHit;
// ���� ������ �Ǵ� ����, Ŭ�� <= ����
typedef struct PDUDie
{
	const BYTE type = DIE;
	DWORD id;
} PDUDie;