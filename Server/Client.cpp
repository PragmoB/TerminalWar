#include <WinSock2.h>
#include <ctime>

#include <list>
#include <mutex>

#include "Background.h"
#include "Client.h"
#include "protocol.h"
#include "interface.h"

#include "Items/Item.h"
#include "Items/Heart.h"
#include "Items/Energy.h"

#include "Skills/ActiveSkill.h"
#include "Skills/PassiveSkill.h"


extern Background background;

SOCKET Client::udp_socket = 0;
const int Client::MOV_DELAY = 100;

Client::Client(ClientContext context, sockaddr_in addr, COORD pos)
: context(context), pos(pos)
{
	this->context.dataBuffer.buf = this->context.buffer;
	this->addr = addr;

	// 자기자신을 인식함
	apply_hello_of(this);

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
	{
		// 유저들에게 인사를 받음
		apply_hello_of(*iter);
	}

	// 아이템 정보 수용
	
	for (std::list<Item*>::iterator iter = background.items.begin();
		iter != background.items.end();
		iter++)
		apply_item_info_of(*iter);
}

int Client::get_level() const
{
	return level;
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
const Skill* Client::get_skill(int idx) const
{
	return skills[idx];
}
int Client::get_len_skills() const
{
	return len_skills;
}
int Client::get_damage_increase_rate() const
{
	return damage_increase_rate;
}

void Client::apply_hello_of(const Client* client)
{
	PDUHello pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.HP = client->get_HP();
	pdu.chracter = client->get_chracter();
	pdu.pos = client->get_pos();
	pdu.len_skills = client->get_len_skills();

	for (int i = 0; i < pdu.len_skills; i++)
	{
		pdu.skills[i] = client->get_skill(i)->get_type();
		pdu.skill_levels[i] = client->get_skill(i)->get_level();
	}

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUHello), NULL);
}
void Client::apply_item_info_of(const Item* item)
{
	PDUItemInfo pdu;
	pdu.item_type = item->get_type();
	pdu.pos = item->get_pos();

	const Energy* energy_item = dynamic_cast<const Energy*>(item);
	if (energy_item)
		pdu.level = energy_item->get_level();

	send(context.socket, (const char*)&pdu, sizeof(PDUItemInfo), NULL);
}
void Client::apply_earn_item_of(const Client* client, const Item* item)
{
	PDUEarnItem pdu;
	pdu.id = client->context.socket;
	pdu.pos = item->get_pos();

	send(context.socket, (const char*)&pdu, sizeof(PDUEarnItem), NULL);
}
void Client::apply_movement_of(const Client* client)
{
	PDUMovRes pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.pos = client->get_pos();
	
	sendto(udp_socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUMovRes), NULL, (struct sockaddr*)&addr, sizeof(addr));
}
void Client::apply_cast_skill_of(const Client* client, SKILL_TYPE skill_type, DIRECTION dir)
{
	PDUCastSkill pdu;
	pdu.id = (DWORD)client->context.socket;
	pdu.skill_type = skill_type;
	pdu.dir = dir;

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUCastSkill), NULL);
}
void Client::apply_upgrade_skill_of(const Client* client, SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type)
{
	PDUUpgradeSkill pdu;
	pdu.id = client->context.socket;
	pdu.skill_type = skill_type;
	pdu.upgraded_skill_type = upgraded_skill_type;

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUUpgradeSkill), NULL);
}
void Client::apply_hit_of(const Client* client, const Skill* skill, bool evaded)
{
	PDUHit pdu;
	pdu.attacker_id = (DWORD)skill->get_owner()->context.socket;
	pdu.victim_id = (DWORD)client->context.socket;
	pdu.skill_type = skill->get_type();
	pdu.evaded = evaded;
	
	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUHit), NULL);
}
void Client::apply_die_of(const Client* client, const Client* attacker)
{
	PDUDie pdu;
	if (client)
	{
		pdu.id = (DWORD)client->context.socket;
		pdu.level = client->get_level();
	}
	if (attacker)
		pdu.attacker_id = (DWORD)attacker->context.socket;
	
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

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
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
bool Client::move(COORD pos)
{
	this->pos = pos;

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_movement_of(this);

	earn_item(pos);

	return true;
}
bool Client::move(DIRECTION dir, bool ignore_mov_cooldown)
{
	const clock_t now = clock();
	
	if (now < next_able_mov_time && !ignore_mov_cooldown) // 이동속도 제한
		return false;
	if (!chracter) // hello를 하기 전이면 움직일 수 없음
		return false;

	if (next_able_mov_time < now + mov_delay)
	{
		static std::mutex m;
		m.lock();
			next_able_mov_time = now + mov_delay; // 움직임을 정해진 시간 마다 한 번으로 제한
		m.unlock();
	}

	// 좌표 반영
	switch (dir)
	{
	case UP: if (1 < pos.Y) pos.Y--; else return false; break;
	case DOWN: if (pos.Y < FIELD_HEIGHT) pos.Y++; else return false; break;
	case LEFT: if (1 < pos.X) pos.X--; else return false; break;
	case RIGHT: if (pos.X < FIELD_WIDTH) pos.X++; else return false; break;
	default: return false; // 방향 값이 잘못된 경우 미승인
	}

	// 고객님들께 반영
	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_movement_of(this);

	earn_item(pos);

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
	for (int i = 0; i < len_skills; i++)
	{
		ActiveSkill* skill = dynamic_cast<ActiveSkill*>(skills[i]);
		if (skill)
			if (skill_type == skill->get_type()) // 대응되는 Skill객체를 찾았다면
			{
				if (skill->castable()) // 사용 가능하다면
				{
					// 백그라운드 스레드 풀에서 피격판정 작업 비동기 처리
					background.cast_skill(skill, dir);

					// 고객님들께 반영
					for (std::list<Client*>::iterator iter = background.clients.begin();
						iter != background.clients.end(); iter++)
						(*iter)->apply_cast_skill_of(this, skill_type, dir);
				}
				break;
			}
	}

	return true;
}
void Client::earn_item(COORD pos)
{
	// pos 위치 아이템들에서 얻은 총 에너지 량
	int energy_gain = 0;

	// 필드 아이템 획득 검사
	for (std::list<Item*>::iterator iter = background.items.begin();
		iter != background.items.end();)
	{
		Item* item = *iter;
		COORD item_pos = item->get_pos();

		// 아이템 획득
		if (item_pos.X == pos.X && item_pos.Y == pos.Y)
		{
			// 플레이어들에게 반영
			for (std::list<Client*>::iterator c_iter = background.clients.begin();
				c_iter != background.clients.end(); c_iter++)
				(*c_iter)->apply_earn_item_of(this, item);

			switch (item->get_type())
			{
			case HEART:
			{
				Heart* heart_item = dynamic_cast<Heart*>(item);
				if (heart_item)
				{
					m_HP.lock();
					HP += heart_item->get_amount();
					m_HP.unlock();
				}
				break;
			}
			case ENERGY:
			{
				Energy* energy_item = dynamic_cast<Energy*>(item);
				if (energy_item)
					energy_gain += energy_item->get_amount();
				break;
			}
			}

			delete item;
			background.items.erase(iter++);
		}
		else
			iter++;
	}

	earn_energy(energy_gain);
	
}
void Client::upgrade_skill(SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type)
{
	if (skill_is_upgradable)
		skill_is_upgradable = false;
	else
		return;

	// 스킬 강화를 포기한 경우
	if (skill_type == UNKNOWN && level - 1 >= 0)
	{
		// 경험치를 정해진 비율로 환수
		earn_energy(REQUIRED_ENERGY[level - 1] * ENERGY_REFUND_RATE / 100);
		return;
	}

	Skill* skill = NULL;
	int idx_skill;
	for (idx_skill = 0; idx_skill < len_skills; idx_skill++)
	{
		if (skills[idx_skill]->get_type() == skill_type)
		{
			skill = skills[idx_skill];
			break;
		}
	}

	// 원래 있던 스킬 레벨업 또는 진화
	if (skill)
	{
		// 원래 있던 스킬이 패시브 스킬이면
		PassiveSkill* passive_skill = dynamic_cast<PassiveSkill*>(skill);
		if (passive_skill)
		{
			// 플레이어에게 적용되어 있던 해당 레벨의 패시브 스킬 능력치 초기화
			speed_increase_rate = 100 * (speed_increase_rate + 100) / (passive_skill->get_speed_rate() + 100) - 100;
		}

		if (skill->level_up())
		{
			// 클라이언트들에게 반영
			for (list<Client*>::iterator iter = background.clients.begin();
				iter != background.clients.end(); iter++)
				(*iter)->apply_upgrade_skill_of(this, skill_type, upgraded_skill_type);
		}
		else if (skill->upgradable_to(upgraded_skill_type))
		{
			// 클라이언트들에게 반영
			for (list<Client*>::iterator iter = background.clients.begin();
				iter != background.clients.end(); iter++)
				(*iter)->apply_upgrade_skill_of(this, skill_type, upgraded_skill_type);

			delete skill;
			skills[idx_skill] = skill = Skill::create_object_by_type(upgraded_skill_type, this);
		}
	}
	// 새로운 스킬 습득
	else if (Skill::get_object_by_type(skill_type)->get_ordinal() == 1 &&
		len_skills < MAX_SKILL)
	{
		skills[len_skills++] = skill = Skill::create_object_by_type(skill_type, this);

		// 클라이언트들에게 반영
		for (list<Client*>::iterator iter = background.clients.begin();
			iter != background.clients.end(); iter++)
			(*iter)->apply_upgrade_skill_of(this, skill_type, upgraded_skill_type);
	}

	// 패시브 스킬이면
	PassiveSkill* passive_skill = dynamic_cast<PassiveSkill*>(skill);
	if (passive_skill)
	{
		// 플레이어에게 해당 레벨의 패시브 스킬 능력치 적용
		speed_increase_rate = (speed_increase_rate + 100) * (passive_skill->get_speed_rate() + 100) / 100 - 100;
		mov_delay = MOV_DELAY * 100 / (speed_increase_rate + 100);
	}
}
void Client::earn_energy(int amount)
{
	m_energy.lock();
	// 스킬 강화 대기중이라면 에너지 획득 불가능
	if (!skill_is_upgradable)
		energy += amount;
	m_energy.unlock();

	level_up();
}
void Client::hit(const ActiveSkill* skill)
{
	// NULL ptr check
	if (!skill)
		return;
	// hello 하기 전이면 안맞음
	if (!chracter)
		return;
	// 자기자신에게 맞다는건 말이 안됨
	if (this == skill->get_owner())
		return;

	// evasion_rate 확률로 회피
	bool evaded = false;
	if (rand() % 100 < evasion_rate)
		evaded = true;
	else
	{
		// 피해입을 데미지 값을 능력치에 따라 연산
		int damage = skill->get_damage();
		damage = damage * (skill->get_owner()->get_damage_increase_rate() + 100) * (100 - defense_rate) / 10000;
		if (damage < 0)
			damage = 0;
		m_HP.lock();
		HP -= damage; // 맞아서 체력 감소
		m_HP.unlock();
	}

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_hit_of(this, skill, evaded);

	if (HP <= 0)
		skill->get_owner()->kill(this);
}
int Client::die(const Client* attacker)
{
	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_die_of(this, attacker);

	// 연결 끊기
	closesocket(context.socket);

	return HUNTING_ENERGY[level];
}

void Client::kill(Client* client)
{
	earn_energy(client->die(this));
}

void Client::level_up()
{
	m_energy.lock();
	// 레벨업 조건 충족
	if (energy >= REQUIRED_ENERGY[level])
	{
		skill_is_upgradable = true;
		level++;
		energy = 0;
		m_energy.unlock();

		// 강화 가능한 스킬 목록들 추려내기
		SKILL_TYPE skill_type_list[sizeof(SKILL_TYPE_LIST) / sizeof(SKILL_TYPE)];
		int len_skill_type_list = sizeof(skill_type_list) / sizeof(SKILL_TYPE);

		for (int i = 0; i < len_skill_type_list; i++)
			skill_type_list[i] = SKILL_TYPE_LIST[i];

		for (int i = 0; i < len_skill_type_list; i++)
		{
			Skill* skill = NULL;
			for (int j = 0; j < len_skills; j++)
			{
				if (skill_type_list[i] == skills[j]->get_type())
					skill = skills[j];
			}

			// 가지고 있으며 (MAX레벨이 아니거나 다음 단계가 있는) 스킬인 경우 : 강화 가능
			if (skill)
			{
				if (skill->get_level() != skill->get_max_level() || skill->upgradable())
					continue;
			}

			// 안가지고 있는 스킬인 경우 : 1차 스킬이고, 거기서 진화된 스킬을 가지고 있지 않은 경우만 강화 가능
			else if (Skill::get_object_by_type(skill_type_list[i])->get_ordinal() == 1)
			{
				int j;
				for (j = 0; j < len_skills; j++)
				{
					if (skills[j]->downgradable_to(skill_type_list[i]))
						break;
				}
				if (j >= len_skills)
					continue;
			}

			SKILL_TYPE temp = skill_type_list[i];
			skill_type_list[i] = skill_type_list[len_skill_type_list - 1];
			skill_type_list[len_skill_type_list - 1] = temp;
			len_skill_type_list--;
			i--;
		}

		int num_upgrade_skill_options = len_skill_type_list < NUM_UPGRADE_SKILL_OPTIONS ?
			len_skill_type_list : NUM_UPGRADE_SKILL_OPTIONS;

		// 스킬 강화 옵션들 설정
		for (int i = 0; i < NUM_UPGRADE_SKILL_OPTIONS; i++)
		{
			skill_option_is_active[i] = false;
			skill_options[i] = SKILL_TYPE::UNKNOWN;
		}
		for (int i = 0; i < num_upgrade_skill_options; i++)
		{
			skill_option_is_active[i] = true;

			// 스킬 강화 옵션 산정
			skill_options[i] = skill_type_list[rand() % len_skill_type_list];

			// 스킬 강화 옵션 중복검사
			for (int j = 0; j < i; j++)
				if (skill_options[i] == skill_options[j])
				{
					i--;
					break;
				}
		}

		// PDU 구성 후 전송
		PDUUpgradeSkillOptionInfo pdu;
		for (int i = 0; i < NUM_UPGRADE_SKILL_OPTIONS; i++)
			pdu.skill_types[i] = skill_options[i];

		send(context.socket, (const char*)&pdu, sizeof(PDUUpgradeSkillOptionInfo), NULL);
	}
	else
		m_energy.unlock();
}
Client::~Client()
{
	// 체력이 남아있으면 자의적으로 죽은것임(attacker가 없음)
	if (HP > 0)
		die();

	for (int i = 0; i < len_skills; i++)
		delete skills[i];
}