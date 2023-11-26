#include <stdlib.h>
#include <ctime>

#include <thread>

#include "Background.h"

#include "Items/Heart.h"
#include "Items/Energy.h"

#include "protocol.h"
#include "interface.h"

Background background;

Background::Background() : skill_queue(3)
{
	// 피격판정 스레드 풀 생성
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
			if (rand() % 5) // 80% 확률
				item = new Energy(rand() % 3 + 1,
					COORD{ rand() % FIELD_WIDTH + 1, rand() % FIELD_HEIGHT + 1 });
			else // 20% 확률
				item = new Heart(COORD{ rand() % FIELD_WIDTH + 1, rand() % FIELD_HEIGHT + 1 });

			// 전송 준비(pdu 구성)
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

			// 전송
			for (list<Client*>::iterator iter = clients.begin();
				iter != clients.end(); iter++)
			{
				Client* client = *iter;
				send(client->context.socket, (const char*)&pdu, sizeof(PDUItemInfo), NULL);
			}
			
			items.push_back(item);
		}
		Sleep(4500);
	}
}
void Background::cast_skill(Skill* skill, DIRECTION dir)
{
	skill_queue.Enqueue(SkillParam{ skill, dir });
}