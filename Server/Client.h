#pragma once

#include <WinSock2.h>
#include <windef.h>
#include <ctime>

#include <iostream>
#include <list>

#include "protocol.h"

using namespace std;

class Client;

// IOCP 데이터 구조체
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
	// 클라이언트 리스트에 push
	static void push(Client* client);
	// 클라이언트 리스트에서 pop
	static uint32_t pop(Client* client);
public:
	Client(POINT pos);

	char getChracter() const;
	POINT getPos() const;
	
	// client가 dir방향으로 움직였음을 반영함
	void apply_movement_of(Client* client, DIRECTION dir);
	// client가 dir방향으로 총 쐈음을 반영함
	void apply_shoot_of(Client* client, DIRECTION dir);
	// client가 접속했음을 반영함
	void apply_hello_of(Client* client);
	// client가 죽었음을 반영함
	void apply_die_of(Client* client);

	// 움직임
	uint32_t move(DIRECTION dir);
	// 총 쏘기
	uint32_t shoot(DIRECTION dir);
	// 다른 클라이언트들에게 전입신고함
	void hello(char chracter);
	void hit();
	void die();
};