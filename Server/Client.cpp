#include <WinSock2.h>
#include <ctime>

#include <list>
#include <mutex>

#include "Background.h"
#include "Client.h"
#include "protocol.h"
#include "interface.h"

std::list<Client*> clients;
extern Background background;

Client::Client(POINT pos)
: pos(pos), last_mov(0), last_shoot(0), chracter(NULL)
{ }

char Client::getChracter() const
{
	return chracter;
}
POINT Client::getPos() const
{	
	return pos;
}

void Client::apply_hello_of(Client* client)
{
	PDUHello pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.chracter = client->getChracter();
	pdu.pos = client->getPos();

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUHello), NULL);
}
void Client::apply_movement_of(Client* client, DIRECTION dir)
{
	PDUMov pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.dir = dir;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUMov), NULL);
}
void Client::apply_shoot_of(Client* client, DIRECTION dir)
{
	PDUShoot pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.dir = dir;

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUShoot), NULL);
}
void Client::apply_hit_of(Client* client)
{
	PDUHit pdu;
	pdu.id = (DWORD)client->context.socket;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUHit), NULL);
}
void Client::apply_die_of(Client* client)
{
	PDUDie pdu;
	pdu.id = (DWORD)client->context.socket;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUDie), NULL);
}

void Client::hello(char chracter)
{
	// hello는 접속 후 한 번만 허용함
	if (this->chracter)
		return;
	// 캐릭터는 영어 알파벳만 허용함
	if (chracter < 0x41 || (0x5a < chracter && chracter < 0x61) || 0x7a < chracter)
		return;

	this->chracter = chracter;

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		// 유저들에게 인사를 주고
		(*iter)->apply_hello_of(this);
		if (this != *iter)
			apply_hello_of(*iter); // 받고
	}
}

uint32_t Client::move(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_mov < 80) // 움직임을 80ms마다 한 번으로 제한
		return FALSE;
	if (!chracter) // hello를 하기 전이면 움직일 수 없음
		return FALSE;

	// 좌표 반영
	switch (dir)
	{
	case UP: if (1 < pos.y) pos.y--; else return FALSE; break;
	case DOWN: if (pos.y < field_height) pos.y++; else return FALSE; break;
	case LEFT: if (1 < pos.x) pos.x--; else return FALSE; break;
	case RIGHT: if (pos.x < field_width) pos.x++; else return FALSE; break;
	default: return FALSE; // 방향 값이 잘못된 경우 미승인
	}

	static std::mutex m;
	m.lock();
	last_mov = now;
	m.unlock();

	// 고객님들께 반영
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	return TRUE;
}
uint32_t Client::shoot(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_shoot < 170) // 사격을 170ms마다 한 번으로 제한
		return FALSE;
	if (!chracter) // hello를 하기 전이면 사격할 수 없음
		return FALSE;
	switch (dir)
	{
	case UP: case DOWN: case LEFT: case RIGHT:
		break;
	default :
		return FALSE; // 방향 값이 잘못된 경우 미승인
	}

	static std::mutex m;
	m.lock();
	last_shoot = now;
	m.unlock();

	// 백그라운드 스레드 풀에 피격판정 작업 위탁
	background.fire_queue.Enqueue(Bullet(pos.x, pos.y, dir));

	// 고객님들께 반영
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_shoot_of(this, dir);

	return TRUE;
}
void Client::hit()
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_hit_of(this);
}
void Client::die()
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_die_of(this);
}