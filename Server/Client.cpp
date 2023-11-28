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

#include "Skills/Skill.h"
#include "Skills/Shoot.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

extern Background background;

Client::Client(ClientContext context, COORD pos)
: context(context), pos(pos)
{
	this->context.dataBuffer.buf = this->context.buffer;

	// �ڱ��ڽ��� �ν���
	apply_hello_of(this);

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
	{
		// �����鿡�� �λ縦 ����
		apply_hello_of(*iter);
	}

	// ������ ���� ����
	
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
		pdu.skills[i] = client->get_active_skill(i)->get_type();
		pdu.skill_levels[i] = client->get_active_skill(i)->get_level();
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
void Client::apply_upgrade_skill_of(const Client* client, SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type)
{
	PDUUpgradeSkill pdu;
	pdu.id = client->context.socket;
	pdu.skill_is_active = true;
	pdu.active_skill_type = skill_type;
	pdu.upgraded_active_skill_type = upgraded_skill_type;

	send(context.socket, reinterpret_cast<const char*>(&pdu), sizeof(PDUUpgradeSkill), NULL);
}
void Client::apply_hit_of(const Client* client, const Skill* skill)
{
	PDUHit pdu;
	pdu.attacker_id = (DWORD)skill->get_owner()->context.socket;
	pdu.victim_id = (DWORD)client->context.socket;
	pdu.skill_type = skill->get_type();
	
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
	// hello�� ���� �� �� ���� �����
	if (this->chracter)
		return;
	// ĳ���ʹ� ���� ���ĺ��� �����
	if (chracter < 0x41 || (0x5a < chracter && chracter < 0x61) || 0x7a < chracter)
		return;

	this->chracter = chracter;

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
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
	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_movement_of(this, dir);

	// �����ۿ��� ���� �� ������ ��
	int energy_gain = 0;

	// �ʵ� ������ ȹ�� �˻�
	for (std::list<Item*>::iterator iter = background.items.begin();
		iter != background.items.end();)
	{
		Item* item = *iter;
		COORD item_pos = item->get_pos();

		// ������ ȹ��
		if (item_pos.X == pos.X && item_pos.Y == pos.Y)
		{
			// �÷��̾�鿡�� �ݿ�
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
		if (skill_type == skill->get_type()) // �����Ǵ� Skill��ü�� ã�Ҵٸ�
		{
			// ��׶��� ������ Ǯ���� �ǰ����� �۾� �񵿱� ó��
			background.cast_skill(skill, dir);
			break;
		}
	}

	// ���Ե鲲 �ݿ�
	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_cast_skill_of(this, skill_type, dir);

	return true;
}
void Client::upgrade_skill(SKILL_TYPE skill_type, SKILL_TYPE upgraded_skill_type)
{
	if (skill_is_upgradable)
		skill_is_upgradable = false;
	else
		return;

	// ��ų ��ȭ�� ������ ���
	if (skill_type == UNKNOWN && level - 1 >= 0)
	{
		// ����ġ�� ������ ������ ȯ��
		earn_energy(REQUIRED_ENERGY[level - 1] * ENERGY_REFUND_RATE / 100);
		return;
	}

	Skill* skill = NULL;
	int idx_skill;
	for (idx_skill = 0; idx_skill < len_active_skills; idx_skill++)
	{
		if (active_skills[idx_skill]->get_type() == skill_type)
		{
			skill = active_skills[idx_skill];
			break;
		}
	}

	// ���� �ִ� ��ų ������ �Ǵ� ��ȭ
	if (skill)
	{
		if (skill->level_up())
		{
			// Ŭ���̾�Ʈ�鿡�� �ݿ�
			for (list<Client*>::iterator iter = background.clients.begin();
				iter != background.clients.end(); iter++)
				(*iter)->apply_upgrade_skill_of(this, skill_type, upgraded_skill_type);
		}
		else if (skill->upgradable_to(upgraded_skill_type))
		{
			// Ŭ���̾�Ʈ�鿡�� �ݿ�
			for (list<Client*>::iterator iter = background.clients.begin();
				iter != background.clients.end(); iter++)
				(*iter)->apply_upgrade_skill_of(this, skill_type, upgraded_skill_type);

			delete skill;
			active_skills[idx_skill] = skill = Skill::create_object_by_type(upgraded_skill_type, this);
		}
	}
	// ���ο� ��ų ����
	else if (Skill::get_object_by_type(skill_type)->get_ordinal() == 1 &&
		len_active_skills < MAX_ACTIVE_SKILL)
	{
		// Ŭ���̾�Ʈ�鿡�� �ݿ�
		for (list<Client*>::iterator iter = background.clients.begin();
			iter != background.clients.end(); iter++)
			(*iter)->apply_upgrade_skill_of(this, skill_type, upgraded_skill_type);

		active_skills[len_active_skills++] = skill = Skill::create_object_by_type(skill_type, this);
	}

}
void Client::earn_energy(int amount)
{
	m_energy.lock();
	// ��ų ��ȭ ������̶�� ������ ȹ�� �Ұ���
	if (!skill_is_upgradable)
		energy += amount;
	m_energy.unlock();

	level_up();
}
void Client::hit(const Skill* skill)
{
	// NULL ptr check
	if (!skill)
		return;

	// hello �ϱ� ���̸� �ȸ���
	if (!chracter)
		return;
	// �ڱ��ڽſ��� �´ٴ°� ���� �ȵ�
	if (this == skill->get_owner())
		return;

	m_HP.lock();
	HP -= skill->get_damage(); // �¾Ƽ� ü�� ����
	m_HP.unlock();

	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_hit_of(this, skill);

	if (HP <= 0)
		skill->get_owner()->kill(this);
}
int Client::die(const Client* attacker)
{
	for (std::list<Client*>::iterator iter = background.clients.begin();
		iter != background.clients.end(); iter++)
		(*iter)->apply_die_of(this, attacker);

	// ���� ����
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
	// ������ ���� ����
	if (energy >= REQUIRED_ENERGY[level])
	{
		skill_is_upgradable = true;
		level++;
		energy = 0;
		m_energy.unlock();

		// ��ȭ ������ ��ų ��ϵ� �߷�����
		int len_skill_type_list = sizeof(SKILL_TYPE_LIST) / sizeof(SKILL_TYPE);
		SKILL_TYPE skill_type_list[sizeof(SKILL_TYPE_LIST) / sizeof(SKILL_TYPE)];
		for (int i = 0; i < len_skill_type_list; i++)
			skill_type_list[i] = SKILL_TYPE_LIST[i];

		for (int i = 0; i < len_skill_type_list; i++)
		{
			Skill* skill = NULL;
			for (int j = 0; j < len_active_skills; j++)
			{
				if (skill_type_list[i] == active_skills[j]->get_type())
					skill = active_skills[j];
			}

			// ������ ������ (MAX������ �ƴϰų� ���� �ܰ谡 �ִ�) ��ų�� ��� : ��ȭ ����
			if (skill)
			{
				if (skill->get_level() != skill->get_max_level() || skill->upgradable())
					continue;
			}

			// �Ȱ����� �ִ� ��ų�� ��� : 1�� ��ų�̰�, �ű⼭ ��ȭ�� ��ų�� ������ ���� ���� ��츸 ��ȭ ����
			else if (Skill::get_object_by_type(skill_type_list[i])->get_ordinal() == 1)
			{
				int j;
				for (j = 0; j < len_active_skills; j++)
				{
					if (active_skills[j]->downgradable_to(skill_type_list[i]))
						break;
				}
				if (j >= len_active_skills)
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

		// ��ų ��ȭ �ɼǵ� ����
		for (int i = 0; i < NUM_UPGRADE_SKILL_OPTIONS; i++)
		{
			skill_option_is_active[i] = false;
			active_skill_options[i] = SKILL_TYPE::UNKNOWN;
		}
		for (int i = 0; i < num_upgrade_skill_options; i++)
		{
			skill_option_is_active[i] = true;

			// ��ų ��ȭ �ɼ� ����
			active_skill_options[i] = skill_type_list[rand() % len_skill_type_list];

			// ��ų ��ȭ �ɼ� �ߺ��˻�
			for (int j = 0; j < i; j++)
				if (active_skill_options[i] == active_skill_options[j])
				{
					i--;
					break;
				}
		}

		// PDU ���� �� ����
		PDUUpgradeSkillOptionInfo pdu;
		for (int i = 0; i < NUM_UPGRADE_SKILL_OPTIONS; i++)
		{
			pdu.skill_is_active[i] = true;
			pdu.active_skill_types[i] = active_skill_options[i];
		}
		send(context.socket, (const char*)&pdu, sizeof(PDUUpgradeSkillOptionInfo), NULL);
	}
	else
		m_energy.unlock();
}
Client::~Client()
{
	// ü���� ���������� ���������� ��������(attacker�� ����)
	if (HP > 0)
		die();

	for (int i = 0; i < len_active_skills; i++)
		delete active_skills[i];
}