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
	// ü��
	int HP;

	// ĳ����(���� ���ĺ� �� ����)
	char chracter;

	// ���� ��ġ
	COORD pos;

	// ���������� ������ �ð�
	clock_t last_mov;

	// ���������� �� �� �ð�
	clock_t last_shoot;

public:
	Client(ClientContext context, COORD pos);

	int getHP() const;
	char getChracter() const;
	COORD getPos() const;
	
	// client�� ���������� �ݿ���
	void apply_hello_of(Client* client);
	// client�� dir�������� ���������� �ݿ���
	void apply_movement_of(Client* client, DIRECTION dir);
	// client�� dir�������� �� ������ �ݿ���
	void apply_shoot_of(Client* client, DIRECTION dir);
	// client�� ó�¾����� �ݿ���
	void apply_hit_of(Client* client);
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