#pragma once

#include <thread>

#include "blocking_queue.h"
#include "Bullet.h"

class Background
{
public:
	BlockingQueue<Bullet> fire_queue;
	std::thread umpires[10];

public:
	Background();

	// 스킬 피격판정 스레드
	void check_attack();
};