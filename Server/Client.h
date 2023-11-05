#pragma once

#include <Winsock2.h>
#include <windef.h>
#include <ctime>

#include <iostream>
#include <list>
#include <vector>

#include "interface.h"
#include "protocol.h"
#include "Skills/Skill.h"

using namespace std;

class Client;

// IOCP ������ ����ü
typedef struct {
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
	int HP = 999;

	// ĳ����(���� ���ĺ� �� ����)
	char chracter = NULL;

	// ���� ��ġ
	COORD pos;

	// �̵� ������ �ð�
	clock_t next_able_mov_time = 0;

	// ������ �ִ� ��Ƽ�� ��ų
	Skill* active_skills[MAX_ACTIVE_SKILL];

	// ������ �ִ� ��Ƽ�� ��ų ��
	int len_active_skills = 0;
public:
	Client(ClientContext context, COORD pos);

	int get_HP() const;
	char get_chracter() const;
	COORD get_pos() const;
	const Skill* get_active_skill(int idx) const;
	int get_len_active_skills() const;
	
	// client�� ���������� �ݿ���
	void apply_hello_of(const Client* client);

	// client�� dir�������� ���������� �ݿ���
	void apply_movement_of(const Client* client, DIRECTION dir);

	// client�� dir�������� �� ������ �ݿ���
	void apply_cast_skill_of(const Client* client, SKILL_TYPE skill_type, DIRECTION dir);

	// client�� skill�� ó�¾����� �ݿ���
	void apply_hit_of(const Client* client, const Skill* skill);

	// client�� �׾����� �ݿ���
	void apply_die_of(const Client* client);

	// time�и��ʸ�ŭ ���� ����(��������)
	void bind(clock_t time);
	// ������
	bool move(DIRECTION dir);
	// ��ų ���
	bool cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	// �ٸ� Ŭ���̾�Ʈ�鿡�� ���ԽŰ���
	void hello(char chracter);

	// client�� skill�� ����
	void hit(const Skill* skill);
	void die();

	~Client();
};