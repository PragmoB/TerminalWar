#include <thread>

#include "Background.h"

Background background;

Background::Background() : fire_queue(50)
{
	// �ǰ����� ������ Ǯ ����
	for (int i = 0; i < 10; i++)
		umpires[i] = std::thread(&Background::check_attack, this);
}
void Background::check_attack()
{
	while (1)
		fire_queue.Dequeue().fire();
}