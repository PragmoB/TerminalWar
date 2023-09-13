#pragma once

const int PORT = 44400;

enum DIRECTION
{	UP, DOWN, LEFT, RIGHT };
enum TYPE_PDU
{	MOV, SHOOT, HIT, HELLO, DIE	};

// 캐릭터 무빙, 클라 <=> 서버
typedef struct PDUMov
{
	const BYTE type = MOV;
	DWORD id;
	DIRECTION dir;
};
// 총알 발사, 클라 <=> 서버
typedef struct PDUShoot
{
	const BYTE type = SHOOT;
	DWORD id;
	DIRECTION dir;
};
// 총알 명중, 클라 <= 서버
typedef struct PDUHit
{
	const BYTE type = HIT;
	POINT pos;
};
// 유저접속, 클라 <= 서버
typedef struct PDUHello
{
	const BYTE type = HELLO;
	DWORD id;
	char chracter;
	POINT pos;
};
