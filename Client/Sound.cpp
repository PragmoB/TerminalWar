#include <Windows.h>
#include <mmsystem.h>
#include <Digitalv.h>

#include <thread>

#include "protocol.h"
#include "blocking_queue.h"

#include "Sound.h"

// 오디오 엔진
Sound sound;

Sound::Sound()
{
	sound_player = std::thread(&Sound::play_sound, this);
}

void Sound::play_sound()
{
	mci_init();

	while (1)
	{
		MCI_PLAY_PARMS mci_play;

		SOUND_PARAM param = sound_queue.Dequeue();

		switch (param.pdu_type)
		{

		case CAST_SKILL:
			switch (param.skill_type)
			{
			case SHOOT:
				mciSendCommand(mci_open_cast_shoot[mci_cur_cast_shoot].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_cast_shoot[mci_cur_cast_shoot].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_cast_shoot %= LEN_CAST_SHOOT_SOUNDCHANNELS;
				break;
			case SLASH: case WIND_SLASH:
				mciSendCommand(mci_open_cast_slash[mci_cur_cast_slash].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_cast_slash[mci_cur_cast_slash].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_cast_slash %= LEN_CAST_SLASH_SOUNDCHANNELS;
				break;
			case LIGHTSABER_SLASH:
				mciSendCommand(mci_open_cast_lightsaber_slash[mci_cur_cast_lightsaber_slash].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_cast_lightsaber_slash[mci_cur_cast_lightsaber_slash].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_cast_lightsaber_slash %= LEN_CAST_LIGHTSABER_SLASH_SOUNDCHANNELS;
				break;
			case ZWEIHANDER_SLASH:
				mciSendCommand(mci_open_cast_zweihander_slash[mci_cur_cast_zweihander_slash].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_cast_zweihander_slash[mci_cur_cast_zweihander_slash].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_cast_zweihander_slash %= LEN_CAST_ZWEIHANDER_SLASH_SOUNDCHANNELS;
				break;
			case WIND:
				mciSendCommand(mci_open_cast_wind[mci_cur_cast_wind].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_cast_wind[mci_cur_cast_wind].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_cast_wind %= LEN_CAST_WIND_SOUNDCHANNELS;
				break;

			}
			break;
		case HIT:
			switch (param.skill_type)
			{
			case SHOOT:
				mciSendCommand(mci_open_hit_shoot[mci_cur_hit_shoot].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_hit_shoot[mci_cur_hit_shoot].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_hit_shoot %= LEN_HIT_SHOOT_SOUNDCHANNELS;
				break;
			case SLASH: case WIND_SLASH:
				mciSendCommand(mci_open_hit_slash[mci_cur_hit_slash].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_hit_slash[mci_cur_hit_slash].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_hit_slash %= LEN_HIT_SLASH_SOUNDCHANNELS;
				break;
			case LIGHTSABER_SLASH:
				mciSendCommand(mci_open_hit_lightsaber_slash[mci_cur_hit_lightsaber_slash].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_hit_lightsaber_slash[mci_cur_hit_lightsaber_slash].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_hit_lightsaber_slash %= LEN_HIT_LIGHTSABER_SLASH_SOUNDCHANNELS;
				break;
			case ZWEIHANDER_SLASH:
				mciSendCommand(mci_open_hit_zweihander_slash[mci_cur_hit_zweihander_slash].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_hit_zweihander_slash[mci_cur_hit_zweihander_slash].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_hit_zweihander_slash %= LEN_HIT_ZWEIHANDER_SLASH_SOUNDCHANNELS;
				break;
			case WIND:
				mciSendCommand(mci_open_hit_wind[mci_cur_hit_wind].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_hit_wind[mci_cur_hit_wind].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_hit_wind %= LEN_HIT_WIND_SOUNDCHANNELS;
				break;
			}
			break;
		}

	}
}
void Sound::mci_init()
{
	// 총쏘기 사운드채널 풀 설정
	for (int i = 0; i < LEN_CAST_SHOOT_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\cast_shoot0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_cast_shoot[i].lpstrElementName = element_name;
		mci_open_cast_shoot[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_shoot[i]);
	}
	for (int i = 0; i < LEN_CAST_SLASH_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\cast_slash0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_cast_slash[i].lpstrElementName = element_name;
		mci_open_cast_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_slash[i]);
	}
	for (int i = 0; i < LEN_CAST_LIGHTSABER_SLASH_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\cast_lightsaber_slash0.wav";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_cast_lightsaber_slash[i].lpstrElementName = element_name;
		mci_open_cast_lightsaber_slash[i].lpstrDeviceType = L"waveaudio";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_lightsaber_slash[i]);
	}
	for (int i = 0; i < LEN_CAST_ZWEIHANDER_SLASH_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\cast_zweihander_slash0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_cast_zweihander_slash[i].lpstrElementName = element_name;
		mci_open_cast_zweihander_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_zweihander_slash[i]);
	}
	for (int i = 0; i < LEN_CAST_WIND_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\cast_wind0.wav";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_cast_wind[i].lpstrElementName = element_name;
		mci_open_cast_wind[i].lpstrDeviceType = L"waveaudio";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_wind[i]);
	}

	// 총맞기 사운드채널 풀 설정
	for (int i = 0; i < LEN_HIT_SHOOT_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\hit_shoot0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_hit_shoot[i].lpstrElementName = element_name;
		mci_open_hit_shoot[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONG_PTR)&mci_open_hit_shoot[i]);
	}
	for (int i = 0; i < LEN_HIT_SLASH_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\hit_slash0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_hit_slash[i].lpstrElementName = element_name;
		mci_open_hit_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_slash[i]);
	}
	for (int i = 0; i < LEN_HIT_LIGHTSABER_SLASH_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\hit_lightsaber_slash0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_hit_lightsaber_slash[i].lpstrElementName = element_name;
		mci_open_hit_lightsaber_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_lightsaber_slash[i]);
	}
	for (int i = 0; i < LEN_HIT_ZWEIHANDER_SLASH_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\hit_zweihander_slash0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_hit_zweihander_slash[i].lpstrElementName = element_name;
		mci_open_hit_zweihander_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_zweihander_slash[i]);
	}
	for (int i = 0; i < LEN_HIT_WIND_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\hit_wind0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_hit_wind[i].lpstrElementName = element_name;
		mci_open_hit_wind[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_wind[i]);
	}
}
void Sound::request(PDU_TYPE pdu_type, SKILL_TYPE skill_type)
{
	sound_queue.Enqueue(SOUND_PARAM{ pdu_type, skill_type });
}

Sound::~Sound()
{
	// 총쏘기 사운드채널 종료
	for (int i = 0; i < LEN_CAST_SHOOT_SOUNDCHANNELS; i++)
		mciSendCommand(mci_open_cast_shoot[i].wDeviceID, MCI_CLOSE, NULL, NULL);

	// 총맞기 사운드채널 종료
	for (int i = 0; i < LEN_HIT_SHOOT_SOUNDCHANNELS; i++)
		mciSendCommand(mci_open_hit_shoot[i].wDeviceID, MCI_CLOSE, NULL, NULL);
}