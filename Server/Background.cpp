#include <stdlib.h>
#include <ctime>

#include <thread>

#include "Background.h"

#include "Skills/ActiveSkill.h"

#include "Items/Heart.h"
#include "Items/Energy.h"

#include "protocol.h"
#include "Values/interface.h"

Background background;

Background::Background() : skill_queue(3)
{
	// �ǰ����� ������ Ǯ ����
	for (int i = 0; i < 10; i++)
		umpires[i] = std::thread(&Background::check_attack, this);

	item_generator = std::thread(&Background::generate_item, this);
}
void Background::check_attack()
{
	while (1)
	{
		SkillParam skill_param = skill_queue.Dequeue();
		skill_param.skill->cast(skill_param.dir);
	}
}

void Background::generate_item()
{
	srand(time(NULL));
	while (1)
	{
		Item* item = NULL;
		if (items.size() < 14)
		{
			if (rand() % 5) // 80% Ȯ��
				item = new Energy(rand() % 3 + 1,
					COORD{ rand() % FIELD_WIDTH + 1, rand() % FIELD_HEIGHT + 1 });
			else // 20% Ȯ��
				item = new Heart(COORD{ rand() % FIELD_WIDTH + 1, rand() % FIELD_HEIGHT + 1 });

			// ���� �غ�(pdu ����)
			Energy* energy_item = NULL;
			PDUItemInfo pdu;
			pdu.item_type = item->get_type();
			pdu.pos = item->get_pos();
			switch (pdu.item_type)
			{
			case ENERGY:
				energy_item = dynamic_cast<Energy*>(item);
				if (energy_item)
					pdu.level = energy_item->get_level();
				break;
			}

			// ����
			for (list<Player*>::iterator iter = players.begin();
				iter != players.end(); iter++)
			{
				Player* player = *iter;
				send(player->context.socket, (const char*)&pdu, sizeof(PDUItemInfo), NULL);
			}
			
			items.push_back(item);
		}
		// Ŭ���̾�Ʈ ���� ���� ���� �ӵ� ����
		size_t num_players = players.size();

		if (num_players)
			Sleep(4000 / num_players);
		else
			Sleep(20);
	}
}
void Background::cast_skill(ActiveSkill* skill, DIRECTION dir)
{
	skill_queue.Enqueue(SkillParam{ skill, dir });
}