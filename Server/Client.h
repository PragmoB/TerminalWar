#pragma once

#include <Winsock2.h>
#include <windef.h>
#include <ctime>

#include <iostream>
#include <list>
#include <vector>
#include <mutex>

#include "interface.h"
#include "protocol.h"
#include "Items/Item.h"
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
	std::mutex m_HP;

	// ����
	int level = 0;
	
	// ����ġ
	int energy = 0;
	std::mutex m_energy;

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

	// ��ų ��ȭ ���� ����
	bool skill_is_upgradable = false;

	// ��ų ��ȭ ������
	union
	{
		SKILL_TYPE active_skill_options[NUM_UPGRADE_SKILL_OPTIONS];
	};
	bool skill_option_is_active[NUM_UPGRADE_SKILL_OPTIONS];
public:
	Client(ClientContext context, COORD pos);

	int get_level() const;
	int get_HP() const;
	char get_chracter() const;
	COORD get_pos() const;
	const Skill* get_active_skill(int idx) const;
	int get_len_active_skills() const;
	
	// client�� ���������� �ݿ���
	void apply_hello_of(const Client* client);
	// item ���� �ݿ�
	void apply_item_info_of(const Item* item);
	// client�� item ȹ�� �ݿ�
	void apply_earn_item_of(const Client* client, const Item* item);
	// client�� dir�������� ���������� �ݿ���
	void apply_movement_of(const Client* client, DIRECTION dir);
	// client�� dir�������� ��ų ����� �ݿ���
	void apply_cast_skill_of(const Client* client, SKILL_TYPE skill_type, DIRECTION dir);
	// client�� skill_type�� ������ �Ǵ� upgraded_skill_type���� ��ȭ������ �ݿ���
	void apply_upgrade_skill_of(const Client* client, SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type = SKILL_TYPE::UNKNOWN);
	// client�� skill�� ó�¾����� �ݿ���
	void apply_hit_of(const Client* client, const Skill* skill);
	// client�� attacker���� �׾����� �ݿ���
	void apply_die_of(const Client* client, const Client* attacker = NULL);

	// time�и��ʸ�ŭ ���� ����(��������)
	void bind(clock_t time);
	// ������
	bool move(DIRECTION dir);
	// ��ų ���
	bool cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	// ��ų ���׷��̵�
	void upgrade_skill(SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type = SKILL_TYPE::UNKNOWN);
	// ������(����ġ) ȹ��
	void earn_energy(int amount);
	// �ٸ� Ŭ���̾�Ʈ�鿡�� ���ԽŰ���
	void hello(char chracter);
	// skill�� ����
	void hit(const Skill* skill);
	// attacker���� ����, ��� ����ġ�� ���ϰ����� ����
	int die(const Client* attacker = NULL);
	// client�� ����
	void kill(Client* client);
	// ������
	void level_up();

	~Client();
};