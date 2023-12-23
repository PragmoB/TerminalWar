#pragma once

#include <Winsock2.h>
#include <windef.h>
#include <ctime>

#include <iostream>
#include <list>
#include <vector>
#include <mutex>

#include "protocol.h"
#include "Items/Item.h"
#include "Skills/ActiveSkill.h"

#include "interface.h"

using namespace std;

class Client;

// IOCP 데이터 구조체
typedef struct {
	OVERLAPPED overlapped;
	SOCKET socket;
	WSABUF dataBuffer;
	char buffer[1024];
} ClientContext;

class Client
{
public:
	ClientContext context;
	static SOCKET udp_socket;
	sockaddr_in addr;
// 캐릭터 능력치
private:
	// 이동 쿨타임 기본값
	static const int MOV_DELAY;
	// 이동 쿨타임 값
	int mov_delay = MOV_DELAY;
	// 이동속도 증가율(백분율)
	int speed_increase_rate = 0;
	// 방어율
	int defense_rate = 0;
	// 데미지 증가율
	int damage_increase_rate = 0;
	// 회피율
	int evasion_rate = 0;
private:
	// 체력
	int HP = 999;
	std::mutex m_HP;

	// 레벨
	int level = 0;
	
	// 경험치
	int energy = 0;
	std::mutex m_energy;

	// 캐릭터(영문 알파벳 한 글자)
	char chracter = NULL;

	// 현재 위치
	COORD pos;

	// 이동 가능한 시간
	clock_t next_able_mov_time = 0;

	// 가지고 있는 액티브 스킬
	Skill* skills[MAX_SKILL];

	// 가지고 있는 스킬 수
	int len_skills = 0;

	// 스킬 강화 가능 여부
	bool skill_is_upgradable = false;

	// 스킬 강화 선택지
	SKILL_TYPE skill_options[NUM_UPGRADE_SKILL_OPTIONS];
	bool skill_option_is_active[NUM_UPGRADE_SKILL_OPTIONS];
public:
	Client(ClientContext context, sockaddr_in IP, COORD pos);

	int get_level() const;
	int get_HP() const;
	char get_chracter() const;
	COORD get_pos() const;
	const Skill* get_skill(int idx) const;
	int get_len_skills() const;
	int get_damage_increase_rate() const;
	
	// client가 접속했음을 반영함
	void apply_hello_of(const Client* client);
	// item 정보 반영
	void apply_item_info_of(const Item* item);
	// client의 item 획득 반영
	void apply_earn_item_of(const Client* client, const Item* item);
	// client가 움직였음을 반영함
	void apply_movement_of(const Client* client);
	// client가 dir방향으로 스킬 사용을 반영함
	void apply_cast_skill_of(const Client* client, SKILL_TYPE skill_type, DIRECTION dir);
	// client의 skill_type이 레벨업 또는 upgraded_skill_type으로 진화했음을 반영함
	void apply_upgrade_skill_of(const Client* client, SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type = SKILL_TYPE::UNKNOWN);
	// client가 skill에 처맞았음을 반영함
	void apply_hit_of(const Client* client, const Skill* skill, bool evaded);
	// client가 attacker에게 죽었음을 반영함
	void apply_die_of(const Client* client, const Client* attacker = NULL);

	// time밀리초만큼 발이 묶임(못움직임)
	void bind(clock_t time);
	// pos 위치로 순간이동
	bool move(COORD pos);
	// dir 방향으로 움직임
	bool move(DIRECTION dir, bool ignore_mov_cooldown = false);
	// 스킬 사용
	bool cast_skill(SKILL_TYPE skill_type, DIRECTION dir);
	// pos위치의 아이템 획득
	void earn_item(COORD pos);
	// 스킬 업그레이드
	void upgrade_skill(SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type = SKILL_TYPE::UNKNOWN);
	// 에너지(경험치) 획득
	void earn_energy(int amount);
	// 다른 클라이언트들에게 전입신고함
	void hello(char chracter);
	// skill에 맞음
	void hit(const ActiveSkill* skill);
	// attacker에게 죽음, 사냥 경험치를 리턴값으로 뱉음
	int die(const Client* attacker = NULL);
	// client를 죽임
	void kill(Client* client);
	// 레벨업
	void level_up();

	~Client();
};