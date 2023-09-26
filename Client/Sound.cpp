#include <Windows.h>
#include <mmsystem.h>
#include <Digitalv.h>

#include <thread>

#include "blocking_queue.h"

#include "Sound.h"

// ����� ����
Sound sound;

enum TYPE_SOUND 
{	SHOOT, HIT	};

Sound::Sound() : mci_cur_shoot(0), mci_cur_hit(0)
{
	sound_player = std::thread(&Sound::play_sound, this);
}

void Sound::play_sound()
{
	mci_init();

	while (1)
	{
		MCI_PLAY_PARMS mci_play;

		switch (sound_queue.Dequeue())
		{
		case SHOOT:
			mciSendCommand(mci_open_shoot[mci_cur_shoot].wDeviceID,
				MCI_SEEK, MCI_SEEK_TO_START, NULL);
			mciSendCommand(mci_open_shoot[mci_cur_shoot].wDeviceID,
				MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

			// ����ä�� �������� ����
			++mci_cur_shoot %= LEN_SHOOT_SOUNDCHANNELS;
			break;

		case HIT:
			mciSendCommand(mci_open_hit[mci_cur_hit].wDeviceID,
				MCI_SEEK, MCI_SEEK_TO_START, NULL);
			mciSendCommand(mci_open_hit[mci_cur_hit].wDeviceID,
				MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mci_play);

			// ����ä�� �������� ����
			++mci_cur_hit %= LEN_HIT_SOUNDCHANNELS;
			break;
		}

	}
}
void Sound::mci_init()
{
	// �ѽ�� ����ä�� Ǯ ����
	for (int i = 0; i < LEN_SHOOT_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\shoot0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_shoot[i].lpstrElementName = element_name;
		mci_open_shoot[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mci_open_shoot[i]);
	}

	// �Ѹ±� ����ä�� Ǯ ����
	for (int i = 0; i < LEN_HIT_SOUNDCHANNELS; i++)
	{
		wchar_t element_name[] = L"soundtracks\\hit0.mp3";
		element_name[wcslen(element_name) - 5] = '0' + i;
		mci_open_hit[i].lpstrElementName = element_name;
		mci_open_hit[i].lpstrDeviceType = L"mpegvideo";
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONG_PTR)&mci_open_hit[i]);
	}
}
void Sound::shoot()
{
	sound_queue.Enqueue(SHOOT);
}
void Sound::hit()
{
	sound_queue.Enqueue(HIT);
}

Sound::~Sound()
{
	// �ѽ�� ����ä�� ����
	for (int i = 0; i < LEN_SHOOT_SOUNDCHANNELS; i++)
		mciSendCommand(mci_open_shoot[i].wDeviceID, MCI_CLOSE, NULL, NULL);

	// �Ѹ±� ����ä�� ����
	for (int i = 0; i < LEN_HIT_SOUNDCHANNELS; i++)
		mciSendCommand(mci_open_hit[i].wDeviceID, MCI_CLOSE, NULL, NULL);
}