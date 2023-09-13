#include <WinSock2.h>
#include <ctime>

#include <list>
#include <mutex>

#include "Client.h"
#include "protocol.h"

std::list<Client*> Client::clients;

Client::Client(POINT pos, char chracter)
: pos(pos), last_mov(0), last_shoot(0), chracter(chracter)
{ }

void Client::push(Client* client)
{
	clients.push_back(client);
}
uint32_t Client::pop(Client* client)
{
	for (list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		if (*iter == client)
		{
			delete client;
			clients.erase(iter);
			return 1;
		}

	return 0;
}

char Client::getChracter() const
{
	return chracter;
}
POINT Client::getPos() const
{	
	return pos;
}

void Client::send(const BYTE* buff, uint32_t len)
{
	WSABUF data;
	data.buf = (CHAR*)&buff;
	data.len = sizeof(PDUMov);

	DWORD send_bytes, flag = 0;
	WSASend(context.socket, &data, 1, &send_bytes, flag, NULL, NULL);
}
void Client::apply_movement_of(Client* client, DIRECTION dir)
{
	PDUMov pdu;
	pdu.id = (DWORD)client;
	pdu.dir = dir;
	
	send(reinterpret_cast<const BYTE*>(&pdu), sizeof(PDUMov));
}
void Client::apply_shoot_of(Client* client, DIRECTION dir)
{
	PDUShoot pdu;
	pdu.id = (DWORD)client;
	pdu.dir = dir;

	send(reinterpret_cast<const BYTE*>(&pdu), sizeof(PDUShoot));
}
void Client::apply_hello_of(Client* client)
{
	PDUHello pdu;
	pdu.id = (DWORD)client;
	pdu.chracter = client->getChracter();
	pdu.pos = client->getPos();

	send(reinterpret_cast<const BYTE*>(&pdu), sizeof(PDUHello));
}

uint32_t Client::move(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_mov < 200) // 움직임을 200ms마다 한 번으로 제한
		return FALSE;

	static std::mutex m;
	m.lock();
	last_mov = now;
	m.unlock();

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	return TRUE;
}
uint32_t Client::shoot(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_shoot < 200) // 사격을 200ms마다 한 번으로 제한
		return FALSE;

	static std::mutex m;
	m.lock();
	last_shoot = now;
	m.unlock();

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	return TRUE;
}

void Client::hello()
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_hello_of(this);
}