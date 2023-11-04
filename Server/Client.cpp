#include <WinSock2.h>
#include <ctime>

#include <list>
#include <mutex>

#include "Background.h"
#include "Client.h"
#include "protocol.h"
#include "interface.h"

#include "Skills/Skill.h"
#include "Skills/Shoot.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

std::list<Client*> clients;
extern Background background;

Client::Client(ClientContext context, COORD pos)
: context(context), pos(pos)
{
	this->context.dataBuffer.buf = this->context.buffer;

	// 일단 스킬 몇 개 장착
	active_skills[len_active_skills++] = new Shoot(this);
	active_skills[len_active_skills++] = new Slash(this);
	active_skills[len_active_skills++] = new LightsaberSlash(this);
	active_skills[len_active_skills++] = new ZweihanderSlash(this);
	active_skills[len_active_skills++] = new WindSlash(this);

	// 자기자신을 인식함
	apply_hello_of(this);

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		// 유저들에게 인사를 받음
		apply_hello_of(*iter);
	}
}

int Client::get_HP() const
{
	return HP;
}
char Client::get_chracter() const
{
	return chracter;
}
COORD Client::get_pos() const
{	
	return pos;
}
const Skill* Client::get_active_skill(int idx) const
{
	return active_skills[idx];
}
int Client::get_len_active_skills() const
{
	return len_active_skills;
}

void Client::apply_hello_of(const Client* client)
{
	PDUHello pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.HP = client->get_HP();
	pdu.chracter = client->get_chracter();
	pdu.pos = client->get_pos();
	pdu.len_skills = client->get_len_active_skills();

	for (int i = 0; i < pdu.len_skills; i++)
	{
		pdu.skills[i] = client->get_active_skill(i)->type;
		pdu.skill_levels[i] = client->get_active_skill(i)->get_level();
	}

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUHello), NULL);
}
void Client::apply_movement_of(const Client* client, DIRECTION dir)
{
	PDUMov pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.dir = dir;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUMov), NULL);
}
void Client::apply_cast_skill_of(const Client* client, SKILL_TYPE skill_type, DIRECTION dir)
{
	PDUCastSkill pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.skill_type = skill_type;
	pdu.dir = dir;

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUCastSkill), NULL);
}
void Client::apply_hit_of(const Client* client, const Skill* skill)
{
	PDUHit pdu;
	pdu.attacker_id = (DWORD)skill->get_owner()->context.socket;
	pdu.victim_id = (DWORD)client->context.socket;
	pdu.skill_type = skill->type;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUHit), NULL);
}
void Client::apply_die_of(const Client* client)
{
	PDUDie pdu;
	pdu.id = (DWORD)client->context.socket;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUDie), NULL);
}

void Client::hello(char chracter)
{
	// hello는 접속 후 한 번만 허용함
	if (this->chracter)
		return;
	// 캐릭터는 영어 알파벳만 허용함
	if (chracter < 0x41 || (0x5a < chracter && chracter < 0x61) || 0x7a < chracter)
		return;

	this->chracter = chracter;

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		// 유저들에게 인사함
		(*iter)->apply_hello_of(this);
	}
}

void Client::bind(clock_t time)
{
	const clock_t now = clock();

	// 이동제한이 목적이므로 변경 후의 이동가능 시간은 무조건 더 커야함
	if (next_able_mov_time < now + time)
		next_able_mov_time = now + time;
}
bool Client::move(DIRECTION dir)
{
	const clock_t now = clock();
	
	if (now < next_able_mov_time) // 이동속도 제한
		return false;
	if (!chracter) // hello를 하기 전이면 움직일 수 없음
		return false;

	// 좌표 반영
	switch (dir)
	{
	case UP: if (1 < pos.Y) pos.Y--; else return false; break;
	case DOWN: if (pos.Y < FIELD_HEIGHT) pos.Y++; else return false; break;
	case LEFT: if (1 < pos.X) pos.X--; else return false; break;
	case RIGHT: if (pos.X < FIELD_WIDTH) pos.X++; else return false; break;
	default: return false; // 방향 값이 잘못된 경우 미승인
	}

	static std::mutex m;
	m.lock();
		next_able_mov_time = now + 80; // 움직임을 80ms마다 한 번으로 제한
	m.unlock();

	// 고객님들께 반영
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	return true;
}
bool Client::cast_skill(SKILL_TYPE skill_type, DIRECTION dir)
{
	const uint32_t now = clock();

	if (!chracter) // hello를 하기 전이면 시전할 수 없음
		return false;
	switch (dir)
	{
	case UP: case DOWN: case LEFT: case RIGHT:
		break;
	default :
		return false; // 방향 값이 잘못된 경우 미승인
	}

	// 사용 요청받은 skill_type에 대응하는 Skill객체를 찾음(skill_type 유효성 검증도 동시에 됨)
	for (int i = 0; i < len_active_skills; i++)
	{
		Skill* skill = active_skills[i];
		if (skill_type == skill->type) // 대응되는 Skill객체를 찾았다면
		{
			// 백그라운드 스레드 풀에서 피격판정 작업 비동기 처리
			background.cast_skill(skill, dir);
			break;
		}
	}

	// 고객님들께 반영
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_cast_skill_of(this, skill_type, dir);

	return true;
}
void Client::hit(const Skill* skill)
{
	// hello 하기 전이면 안맞음
	if (!chracter)
		return;
	// 자기자신에게 맞다는건 말이 안됨
	if (this == skill->get_owner())
		return;

	HP -= skill->get_damage(); // 맞아서 체력 감소
	if (HP <= 0)
	{
		// 연결 끊기
		closesocket(context.socket);
	}
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_hit_of(this, skill);
}
void Client::die()
{
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_die_of(this);
}