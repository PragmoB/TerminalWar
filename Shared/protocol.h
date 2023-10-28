#pragma once

const int PORT = 44400;

enum DIRECTION
{	UP = 1, DOWN, LEFT, RIGHT };
enum TYPE_PDU
{	HELLO, MOV, SHOOT, HIT, DIE	};

// 유저접속, 클라 <= 서버
typedef struct PDUHello
{
	const BYTE type = HELLO;
	DWORD id;
	int HP;
	char chracter;
	COORD pos;
} PDUHello;
// 캐릭터 무빙, 클라 <=> 서버
typedef struct PDUMov
{
	const BYTE type = MOV;
	DWORD id;
	DIRECTION dir;
} PDUMov;
// 총알 발사, 클라 <=> 서버
typedef struct PDUShoot
{
	const BYTE type = SHOOT;
	DWORD id;
	DIRECTION dir;
} PDUShoot;
// 총알 명중, 클라 <= 서버
typedef struct PDUHit
{
	const BYTE type = HIT;
	DWORD id;
	int damage;
} PDUHit;
// 접속 끊어짐 또는 죽음, 클라 <= 서버
typedef struct PDUDie
{
	const BYTE type = DIE;
	DWORD id;
} PDUDie;