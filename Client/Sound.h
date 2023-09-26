#pragma once

#include <mmsystem.h>

#include <thread>

#include "blocking_queue.h"

#define LEN_SHOOT_SOUNDCHANNELS 8
#define LEN_HIT_SOUNDCHANNELS 3

/*
 * �������� ����ϴ� ����� ���� ���Ǻ�.
 * MCI(������� API) ����̽��� ��� ������ �����带 ���� �� ���⿡
 * ��¿ �� ���� �����ϰ� ��������.
 */
class Sound
{
private:
	MCI_OPEN_PARMS mci_open_shoot[LEN_SHOOT_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit[LEN_HIT_SOUNDCHANNELS];

	// ���� ���õ� �ѽ�� ����ä�� ��ȣ
	int mci_cur_shoot;
	// ���� ���õ� �Ѹ±� ����ä�� ��ȣ
	int mci_cur_hit;
	
	// ���� ��� ��û ť
	BlockingQueue<int> sound_queue;

	// ���� ��� ������
	std::thread sound_player;
	void play_sound();

	// MCI ����̽� �ʱ�ȭ
	void mci_init();

public:
	Sound();
	void shoot();
	void hit();
	~Sound();
};