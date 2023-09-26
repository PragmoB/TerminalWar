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
	// hello�� ���� �� �� ���� �����
	if (this->chracter)
		return;
	// ĳ���ʹ� ���� ���ĺ��� �����
	if (chracter < 0x41 || (0x5a < chracter && chracter < 0x61) || 0x7a < chracter)
		return;

	this->chracter = chracter;

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		// �����鿡�� �λ縦 �ְ�
		(*iter)->apply_hello_of(this);
		if (this != *iter)
			apply_hello_of(*iter); // �ް�
	}
}

uint32_t Client::move(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_mov < 80) // �������� 80ms���� �� ������ ����
		return FALSE;
	if (!chracter) // hello�� �ϱ� ���̸� ������ �� ����
		return FALSE;

	// ��ǥ �ݿ�
	switch (dir)
	{
	case UP: if (1 < pos.y) pos.y--; else return FALSE; break;
	case DOWN: if (pos.y < field_height) pos.y++; else return FALSE; break;
	case LEFT: if (1 < pos.x) pos.x--; else return FALSE; break;
	case RIGHT: if (pos.x < field_width) pos.x++; else return FALSE; break;
	default: return FALSE; // ���� ���� �߸��� ��� �̽���
	}

	static std::mutex m;
	m.lock();
	last_mov = now;
	m.unlock();

	// ���Ե鲲 �ݿ�
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	return TRUE;
}
uint32_t Client::shoot(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_shoot < 170) // ����� 170ms���� �� ������ ����
		return FALSE;
	if (!chracter) // hello�� �ϱ� ���̸� ����� �� ����
		return FALSE;
	switch (dir)
	{
	case UP: case DOWN: case LEFT: case RIGHT:
		break;
	default :
		return FALSE; // ���� ���� �߸��� ��� �̽���
	}

	static std::mutex m;
	m.lock();
	last_shoot = now;
	m.unlock();

	// ��׶��� ������ Ǯ�� �ǰ����� �۾� ��Ź
	background.fire_queue.Enqueue(Bullet(pos.x, pos.y, dir));

	// ���Ե鲲 �ݿ�
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