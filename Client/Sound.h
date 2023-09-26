#pragma once

#include <mmsystem.h>

#include <thread>

#include "blocking_queue.h"

#define LEN_SHOOT_SOUNDCHANNELS 8
#define LEN_HIT_SOUNDCHANNELS 3

/*
 * 전역에서 사용하는 오디오 엔진 정의부.
 * MCI(사운드재생 API) 디바이스의 사용 범위는 스레드를 넘을 수 없기에
 * 어쩔 수 없이 복잡하게 구현했음.
 */
class Sound
{
private:
	MCI_OPEN_PARMS mci_open_shoot[LEN_SHOOT_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit[LEN_HIT_SOUNDCHANNELS];

	// 현재 선택된 총쏘기 사운드채널 번호
	int mci_cur_shoot;
	// 현재 선택된 총맞기 사운드채널 번호
	int mci_cur_hit;
	
	// 사운드 재생 요청 큐
	BlockingQueue<int> sound_queue;

	// 사운드 재생 스레드
	std::thread sound_player;
	void play_sound();

	// MCI 디바이스 초기화
	void mci_init();

public:
	Sound();
	void shoot();
	void hit();
	~Sound();
};