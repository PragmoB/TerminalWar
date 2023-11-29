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
		case HELLO: // 배경음악 재생
			mciSendCommand(mci_open_bgm.wDeviceID,
				MCI_SEEK, MCI_SEEK_TO_START, NULL);
			mciSendCommand(mci_open_bgm.wDeviceID,
				MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&mci_play);
			break;

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
			case SNIPE:
				mciSendCommand(mci_open_cast_snipe[mci_cur_cast_snipe].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_cast_snipe[mci_cur_cast_snipe].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_cast_snipe %= LEN_CAST_SNIPE_SOUNDCHANNELS;
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
			case SNIPE:
				mciSendCommand(mci_open_hit_snipe[mci_cur_hit_snipe].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_hit_snipe[mci_cur_hit_snipe].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_hit_snipe %= LEN_HIT_SNIPE_SOUNDCHANNELS;
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

		case EARN_ITEM:
			switch (param.item_type)
			{
			case ENERGY:
				mciSendCommand(mci_open_earn_energy[mci_cur_earn_energy].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_earn_energy[mci_cur_earn_energy].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_earn_energy %= LEN_EARN_ENERGY_SOUNDCHANNELS;
				break;

			case HEART:
				mciSendCommand(mci_open_earn_heart[mci_cur_earn_heart].wDeviceID,
					MCI_SEEK, MCI_SEEK_TO_START, NULL);
				mciSendCommand(mci_open_earn_heart[mci_cur_earn_heart].wDeviceID,
					MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

				// 사운드채널 돌려가며 쓰기
				++mci_cur_earn_heart %= LEN_EARN_HEART_SOUNDCHANNELS;
				break;
			}
			break;

		case UPGRADE_SKILL_OPTION_INFO:
			mciSendCommand(mci_open_level_up.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
			mciSendCommand(mci_open_level_up.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);
			break;
		}

	}
}
void Sound::mci_init()
{
	wchar_t soundtrack_root[] = L"soundtracks";
	wchar_t buff[200];

	// 배경음악 사운드채널 설정
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\bgm.mp3");
		mci_open_bgm.lpstrElementName = buff;
		mci_open_bgm.lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_bgm);
	}

	// 스킬 시전 사운드채널 풀 설정
	for (int i = 0; i < LEN_CAST_SHOOT_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\cast_shoot0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_cast_shoot[i].lpstrElementName = buff;
		mci_open_cast_shoot[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_shoot[i]);
	}
	for (int i = 0; i < LEN_CAST_SNIPE_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\cast_snipe0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_cast_snipe[i].lpstrElementName = buff;
		mci_open_cast_snipe[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_snipe[i]);
	}
	for (int i = 0; i < LEN_CAST_SLASH_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\cast_slash0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_cast_slash[i].lpstrElementName = buff;
		mci_open_cast_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_slash[i]);
	}
	for (int i = 0; i < LEN_CAST_LIGHTSABER_SLASH_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\cast_lightsaber_slash0.wav");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_cast_lightsaber_slash[i].lpstrElementName = buff;
		mci_open_cast_lightsaber_slash[i].lpstrDeviceType = L"waveaudio";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_lightsaber_slash[i]);
	}
	for (int i = 0; i < LEN_CAST_ZWEIHANDER_SLASH_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\cast_zweihander_slash0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_cast_zweihander_slash[i].lpstrElementName = buff;
		mci_open_cast_zweihander_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_zweihander_slash[i]);
	}
	for (int i = 0; i < LEN_CAST_WIND_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\cast_wind0.wav");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_cast_wind[i].lpstrElementName = buff;
		mci_open_cast_wind[i].lpstrDeviceType = L"waveaudio";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_cast_wind[i]);
	}

	// 스킬 피격 사운드채널 풀 설정
	for (int i = 0; i < LEN_HIT_SHOOT_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\hit_shoot0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_hit_shoot[i].lpstrElementName = buff;
		mci_open_hit_shoot[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONG_PTR)&mci_open_hit_shoot[i]);
	}
	for (int i = 0; i < LEN_HIT_SNIPE_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\hit_snipe0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_hit_snipe[i].lpstrElementName = buff;
		mci_open_hit_snipe[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONG_PTR)&mci_open_hit_snipe[i]);
	}
	for (int i = 0; i < LEN_HIT_SLASH_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\hit_slash0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_hit_slash[i].lpstrElementName = buff;
		mci_open_hit_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_slash[i]);
	}
	for (int i = 0; i < LEN_HIT_LIGHTSABER_SLASH_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\hit_lightsaber_slash0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_hit_lightsaber_slash[i].lpstrElementName = buff;
		mci_open_hit_lightsaber_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_lightsaber_slash[i]);
	}
	for (int i = 0; i < LEN_HIT_ZWEIHANDER_SLASH_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\hit_zweihander_slash0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_hit_zweihander_slash[i].lpstrElementName = buff;
		mci_open_hit_zweihander_slash[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_zweihander_slash[i]);
	}
	for (int i = 0; i < LEN_HIT_WIND_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\hit_wind0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_hit_wind[i].lpstrElementName = buff;
		mci_open_hit_wind[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_hit_wind[i]);
	}
	// 레벨업 사운드채널 설정
	{
		wchar_t element_name[] = L"\\level_up.mp3";
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, element_name);

		mci_open_level_up.lpstrElementName = buff;
		mci_open_level_up.lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_level_up);
	}
	// 아이템 획득 사운드채널 풀 설정
	for (int i = 0; i < LEN_EARN_ENERGY_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\earn_energy0.wav");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_earn_energy[i].lpstrElementName = buff;
		mci_open_earn_energy[i].lpstrDeviceType = L"waveaudio";
		int bret = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_earn_energy[i]);
		bret = 1;
	}
	for (int i = 0; i < LEN_EARN_HEART_SOUNDCHANNELS; i++)
	{
		wcscpy_s(buff, soundtrack_root);
		wcscat_s(buff, L"\\earn_heart0.mp3");
		buff[wcslen(buff) - 5] = '0' + i;

		mci_open_earn_heart[i].lpstrElementName = buff;
		mci_open_earn_heart[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_earn_heart[i]);
	}
}
void Sound::request(PDU_TYPE pdu_type, SKILL_TYPE skill_type)
{
	SOUND_PARAM param;
	param.pdu_type = pdu_type;
	param.skill_type = skill_type;
	sound_queue.Enqueue(param);
}
void Sound::request(PDU_TYPE pdu_type, ITEM_TYPE item_type)
{
	SOUND_PARAM param;
	param.pdu_type = pdu_type;
	param.item_type = item_type;
	sound_queue.Enqueue(param);
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