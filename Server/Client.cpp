#include <WinSock2.h>
#include <ctime>

#include <list>
#include <mutex>

#include "Client.h"
#include "protocol.h"

std::list<Client*> Client::clients;

Client::Client(POINT pos)
: pos(pos), last_mov(0), last_shoot(0), chracter(NULL)
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

void Client::send(const char* buff, uint32_t len)
{
	/*
	WSABUF data;
	data.buf = (CHAR*)&buff;
	data.len = len;

	DWORD send_bytes, flag = 0;
	if (WSASend(context.socket, &data, 1, &send_bytes, flag, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cerr << " Failed to send : " << WSAGetLastError() << endl;
		}
	}*/

	::send(context.socket, buff, len, NULL);
}
void Client::apply_movement_of(Client* client, DIRECTION dir)
{
	PDUMov pdu;
	pdu.id = (DWORD)client;
	pdu.dir = dir;
	
	send(reinterpret_cast<const char*>(&pdu), sizeof(PDUMov));
}
void Client::apply_shoot_of(Client* client, DIRECTION dir)
{
	PDUShoot pdu;
	pdu.id = (DWORD)client;
	pdu.dir = dir;

	send(reinterpret_cast<const char*>(&pdu), sizeof(PDUShoot));
}
void Client::apply_hello_of(Client* client)
{
	PDUHello pdu;
	pdu.id = (DWORD)client;
	pdu.chracter = client->getChracter();
	pdu.pos = client->getPos();

	send(reinterpret_cast<const char*>(&pdu), sizeof(PDUHello));
}
void Client::apply_die_of(Client* client)
{
	PDUDie pdu;
	pdu.id = (DWORD)client;
	
	send(reinterpret_cast<const char*>(&pdu), sizeof(PDUDie));
}

void Client::hello(char chracter)
{
	// hello는 접속 후 한 번만 허용함
	if (this->chracter)
		return;

	this->chracter = chracter;

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		(*iter)->apply_hello_of(this);
		if (this != *iter)
			apply_hello_of(*iter);
	}
}

uint32_t Client::move(DIRECTION dir)
{
	const uint32_t now = clock();
	
	if (now - last_mov < 80) // 움직임을 80ms마다 한 번으로 제한
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
	
	if (now - last_shoot < 170) // 사격을 170ms마다 한 번으로 제한
		return FALSE;

	static std::mutex m;
	m.lock();
	last_shoot = now;
	m.unlock();

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_shoot_of(this, dir);

	return TRUE;
}

void Client::die()
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_die_of(this);
}