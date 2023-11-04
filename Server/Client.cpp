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

	// �ϴ� ��ų �� �� ����
	active_skills[len_active_skills++] = new Shoot(this);
	active_skills[len_active_skills++] = new Slash(this);
	active_skills[len_active_skills++] = new LightsaberSlash(this);
	active_skills[len_active_skills++] = new ZweihanderSlash(this);
	active_skills[len_active_skills++] = new WindSlash(this);

	// �ڱ��ڽ��� �ν���
	apply_hello_of(this);

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		// �����鿡�� �λ縦 ����
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
	// hello�� ���� �� �� ���� �����
	if (this->chracter)
		return;
	// ĳ���ʹ� ���� ���ĺ��� �����
	if (chracter < 0x41 || (0x5a < chracter && chracter < 0x61) || 0x7a < chracter)
		return;

	this->chracter = chracter;

	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
	{
		// �����鿡�� �λ���
		(*iter)->apply_hello_of(this);
	}
}

void Client::bind(clock_t time)
{
	const clock_t now = clock();

	// �̵������� �����̹Ƿ� ���� ���� �̵����� �ð��� ������ �� Ŀ����
	if (next_able_mov_time < now + time)
		next_able_mov_time = now + time;
}
bool Client::move(DIRECTION dir)
{
	const clock_t now = clock();
	
	if (now < next_able_mov_time) // �̵��ӵ� ����
		return false;
	if (!chracter) // hello�� �ϱ� ���̸� ������ �� ����
		return false;

	// ��ǥ �ݿ�
	switch (dir)
	{
	case UP: if (1 < pos.Y) pos.Y--; else return false; break;
	case DOWN: if (pos.Y < FIELD_HEIGHT) pos.Y++; else return false; break;
	case LEFT: if (1 < pos.X) pos.X--; else return false; break;
	case RIGHT: if (pos.X < FIELD_WIDTH) pos.X++; else return false; break;
	default: return false; // ���� ���� �߸��� ��� �̽���
	}

	static std::mutex m;
	m.lock();
		next_able_mov_time = now + 80; // �������� 80ms���� �� ������ ����
	m.unlock();

	// ���Ե鲲 �ݿ�
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	return true;
}
bool Client::cast_skill(SKILL_TYPE skill_type, DIRECTION dir)
{
	const uint32_t now = clock();

	if (!chracter) // hello�� �ϱ� ���̸� ������ �� ����
		return false;
	switch (dir)
	{
	case UP: case DOWN: case LEFT: case RIGHT:
		break;
	default :
		return false; // ���� ���� �߸��� ��� �̽���
	}

	// ��� ��û���� skill_type�� �����ϴ� Skill��ü�� ã��(skill_type ��ȿ�� ������ ���ÿ� ��)
	for (int i = 0; i < len_active_skills; i++)
	{
		Skill* skill = active_skills[i];
		if (skill_type == skill->type) // �����Ǵ� Skill��ü�� ã�Ҵٸ�
		{
			// ��׶��� ������ Ǯ���� �ǰ����� �۾� �񵿱� ó��
			background.cast_skill(skill, dir);
			break;
		}
	}

	// ���Ե鲲 �ݿ�
	for (std::list<Client*>::iterator iter = clients.begin();
		iter != clients.end(); iter++)
		(*iter)->apply_cast_skill_of(this, skill_type, dir);

	return true;
}
void Client::hit(const Skill* skill)
{
	// hello �ϱ� ���̸� �ȸ���
	if (!chracter)
		return;
	// �ڱ��ڽſ��� �´ٴ°� ���� �ȵ�
	if (this == skill->get_owner())
		return;

	HP -= skill->get_damage(); // �¾Ƽ� ü�� ����
	if (HP <= 0)
	{
		// ���� ����
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