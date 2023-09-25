#include <thread>

#include "Background.h"

Background background;

Background::Background() : fire_queue(50)
{
	// 피격판정 스레드 풀 생성
	for (int i = 0; i < 10; i++)
		umpires[i] = std::thread(&Background::check_attack, this);
}
void Background::check_attack()
{
	while (1)
		fire_queue.Dequeue().fire();
}