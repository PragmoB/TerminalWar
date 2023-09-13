#pragma once

const int PORT = 44400;

enum DIRECTION
{	UP, DOWN, LEFT, RIGHT };
enum TYPE_PDU
{	MOV, SHOOT, HIT, HELLO, DIE	};

// ĳ���� ����, Ŭ�� <=> ����
typedef struct PDUMov
{
	const BYTE type = MOV;
	DWORD id;
	DIRECTION dir;
};
// �Ѿ� �߻�, Ŭ�� <=> ����
typedef struct PDUShoot
{
	const BYTE type = SHOOT;
	DWORD id;
	DIRECTION dir;
};
// �Ѿ� ����, Ŭ�� <= ����
typedef struct PDUHit
{
	const BYTE type = HIT;
	POINT pos;
};
// ��������, Ŭ�� <= ����
typedef struct PDUHello
{
	const BYTE type = HELLO;
	DWORD id;
	char chracter;
	POINT pos;
};
