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

// IOCP 데이터 구조체
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
	// 체력
	int HP;

	// 캐릭터(영문 알파벳 한 글자)
	char chracter;

	// 현재 위치
	COORD pos;

	// 마지막으로 움직인 시간
	clock_t last_mov;

	// 마지막으로 총 쏜 시간
	clock_t last_shoot;

	// 가지고 있는 액티브 스킬
	Skill* active_skills[MAX_ACTIVE_SKILL];

	// 가지고 있는 액티브 스킬 수
	int len_active_skills;
public:
	Client(ClientContext context, COORD pos);

	int get_HP() const;
	char get_chracter() const;
	COORD get_pos() const;
	const Skill* get_active_skill(int idx) const;
	int get_len_active_skills() const;
	
	// client가 접속했음을 반영함
	void apply_hello_of(const Client* client);

	// client가 dir방향으로 움직였음을 반영함
	void apply_movement_of(const Client* client, DIRECTION dir);

	// client가 dir방향으로 총 쐈음을 반영함
	void apply_cast_skill_of(const Client* client, SKILL_TYPE skill_type, DIRECTION dir);

	// client가 skill에 처맞았음을 반영함
	void apply_hit_of(const Client* client, const Skill* skill);

	// client가 죽었음을 반영함
	void apply_die_of(const Client* client);

	// 움직임
	bool move(DIRECTION dir);
	// 스킬 사용
	bool cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	// 다른 클라이언트들에게 전입신고함
	void hello(char chracter);

	// client의 skill에 맞음
	void hit(const Skill* skill);
	void die();
};