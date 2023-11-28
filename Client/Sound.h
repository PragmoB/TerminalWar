#pragma once

#include <mmsystem.h>

#include <thread>

#include "protocol.h"
#include "blocking_queue.h"

#define LEN_CAST_SHOOT_SOUNDCHANNELS 5
#define LEN_CAST_SNIPE_SOUNDCHANNELS 5
#define LEN_CAST_SLASH_SOUNDCHANNELS 5
#define LEN_CAST_LIGHTSABER_SLASH_SOUNDCHANNELS 5
#define LEN_CAST_ZWEIHANDER_SLASH_SOUNDCHANNELS 5
#define LEN_CAST_WIND_SOUNDCHANNELS 5

#define LEN_HIT_SHOOT_SOUNDCHANNELS 3
#define LEN_HIT_SNIPE_SOUNDCHANNELS 5
#define LEN_HIT_SLASH_SOUNDCHANNELS 5
#define LEN_HIT_LIGHTSABER_SLASH_SOUNDCHANNELS 5
#define LEN_HIT_ZWEIHANDER_SLASH_SOUNDCHANNELS 5
#define LEN_HIT_WIND_SOUNDCHANNELS 5

typedef struct {
	PDU_TYPE pdu_type;
	SKILL_TYPE skill_type;
} SOUND_PARAM;
/*
 * 전역에서 사용하는 오디오 엔진 정의부.
 * MCI(사운드재생 API) 디바이스의 사용 범위는 스레드를 넘을 수 없기에
 * 어쩔 수 없이 복잡하게 구현했음.
 */
class Sound
{
private:
	MCI_OPEN_PARMS mci_open_bgm;

	MCI_OPEN_PARMS mci_open_cast_shoot[LEN_CAST_SHOOT_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_cast_snipe[LEN_CAST_SNIPE_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_cast_slash[LEN_CAST_SLASH_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_cast_lightsaber_slash[LEN_CAST_LIGHTSABER_SLASH_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_cast_zweihander_slash[LEN_CAST_ZWEIHANDER_SLASH_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_cast_wind[LEN_CAST_WIND_SOUNDCHANNELS];

	// 현재 선택된 스킬 시전 사운드채널 번호
	int mci_cur_cast_shoot = 0;
	int mci_cur_cast_snipe = 0;
	int mci_cur_cast_slash = 0;
	int mci_cur_cast_lightsaber_slash = 0;
	int mci_cur_cast_zweihander_slash = 0;
	int mci_cur_cast_wind = 0;

	MCI_OPEN_PARMS mci_open_hit_shoot[LEN_HIT_SHOOT_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit_snipe[LEN_HIT_SNIPE_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit_slash[LEN_HIT_SLASH_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit_lightsaber_slash[LEN_HIT_LIGHTSABER_SLASH_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit_zweihander_slash[LEN_HIT_ZWEIHANDER_SLASH_SOUNDCHANNELS];
	MCI_OPEN_PARMS mci_open_hit_wind[LEN_HIT_WIND_SOUNDCHANNELS];

	// 현재 선택된 스킬 피격 사운드채널 번호
	int mci_cur_hit_shoot = 0;
	int mci_cur_hit_snipe = 0;
	int mci_cur_hit_slash = 0;
	int mci_cur_hit_lightsaber_slash = 0;
	int mci_cur_hit_zweihander_slash = 0;
	int mci_cur_hit_wind = 0;
	
	// 사운드 재생 요청 큐
	BlockingQueue<SOUND_PARAM> sound_queue;

	// 사운드 재생 스레드
	std::thread sound_player;
	void play_sound();

	// MCI 디바이스 초기화
	void mci_init();

public:
	Sound();
	void request(PDU_TYPE pdu_type, SKILL_TYPE skill_type = UNKNOWN);
	~Sound();
};