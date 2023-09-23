#pragma once

#include <WinSock2.h>
#include <windef.h>
#include <ctime>

#include <iostream>
#include <list>

#include "protocol.h"

using namespace std;

class Client;

// IOCP ������ ����ü
typedef struct tagClientContext {
	OVERLAPPED overlapped;
	Client* client;
	SOCKET socket;
	WSABUF dataBuffer;
	char buffer[1024];
} ClientContext;

class Client
{
public:
	ClientContext context;
private:
	static std::list<Client*> clients;
	char chracter;
	POINT pos;
	clock_t last_mov;
	clock_t last_shoot;

public:
	// Ŭ���̾�Ʈ ����Ʈ�� push
	static void push(Client* client);
	// Ŭ���̾�Ʈ ����Ʈ���� pop
	static uint32_t pop(Client* client);
public:
	Client(POINT pos);

	char getChracter() const;
	POINT getPos() const;
	
	// client�� dir�������� ���������� �ݿ���
	void apply_movement_of(Client* client, DIRECTION dir);
	// client�� dir�������� �� ������ �ݿ���
	void apply_shoot_of(Client* client, DIRECTION dir);
	// client�� ���������� �ݿ���
	void apply_hello_of(Client* client);
	// client�� �׾����� �ݿ���
	void apply_die_of(Client* client);

	// ������
	uint32_t move(DIRECTION dir);
	// �� ���
	uint32_t shoot(DIRECTION dir);
	// �ٸ� Ŭ���̾�Ʈ�鿡�� ���ԽŰ���
	void hello(char chracter);
	void hit();
	void die();
};