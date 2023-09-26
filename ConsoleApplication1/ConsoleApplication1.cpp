#include <Windows.h>
#include <stdio.h>
#include <stdexcept>

#ifdef _UNICODE
#define stprintf_s swprintf_s
#else
#define stprintf_s sprintf_s
#endif

class Player {
public:
	Player(LPCTSTR lpFileName) {
		MCI_OPEN_PARMS openp;
		MCI_SET_PARMS setp;
		openp.dwCallback = NULL;
		openp.lpstrDeviceType = reinterpret_cast<LPCTSTR>(MCI_ALL_DEVICE_ID);
		openp.lpstrElementName = lpFileName;
		TCHAR name[32];
		static int alias = 0;
		stprintf_s(name, TEXT("alias%08d"), alias++);
		openp.lpstrAlias = name;
		checkerror(mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT | MCI_OPEN_SHAREABLE | MCI_OPEN_ALIAS, reinterpret_cast<DWORD_PTR>(&openp)));
		_device = openp.wDeviceID;
		setp.dwCallback = NULL;
		setp.dwTimeFormat = 0;
		if (mciSendCommand(openp.wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, reinterpret_cast<DWORD_PTR>(&setp)) == DWORD(-1)) {
			close();
			throw std::runtime_error("Can't open MCI device");
		}
	}

	Player(Player const&) = delete;

	Player(Player&& other) {
		_device = other._device;
		other._device = 0;
	}

	~Player() {
		if (_device != 0) {
			close();
		}
	}

	void play(HWND hWndNotify) {
		MCI_PLAY_PARMS params;
		params.dwCallback = reinterpret_cast<DWORD_PTR>(hWndNotify);
		params.dwFrom = NULL;
		params.dwTo = NULL;
		checkerror(mciSendCommand(_device, MCI_PLAY, (hWndNotify != 0) ? MCI_NOTIFY : 0, reinterpret_cast<DWORD_PTR>(&params)));
	}

	void rewind() {
		MCI_SEEK_PARMS params;
		checkerror(mciSendCommand(_device, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_START, reinterpret_cast<DWORD_PTR>(&params)));
	}

	void pause() {
		MCI_GENERIC_PARMS params;
		params.dwCallback = NULL;
		checkerror(mciSendCommand(_device, MCI_PAUSE, MCI_WAIT, reinterpret_cast<DWORD_PTR>(&params)));
	}

	void stop() {
		MCI_GENERIC_PARMS params;
		params.dwCallback = NULL;
		checkerror(mciSendCommand(_device, MCI_STOP, MCI_WAIT, reinterpret_cast<DWORD_PTR>(&params)));
	}

	MCIDEVICEID device() const { return _device; }

private:
	MCIDEVICEID _device;

	static void checkerror(MCIERROR code) {
		if (code != 0) {
			char buffer[260];
			mciGetErrorStringA(code, buffer, sizeof(buffer) - 1);
			throw std::runtime_error(buffer);
		}
	}

	void close() {
		MCI_GENERIC_PARMS params;
		params.dwCallback = NULL;
		checkerror(mciSendCommand(_device, MCI_CLOSE, MCI_WAIT, reinterpret_cast<DWORD_PTR>(&params)));
	}

};


#include <map>
#include <string>
#include <mutex>

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

class Repeater {
public:

	Repeater(LPCTSTR fn) : fn(fn) {
		hWnd = CreateWindowEx(0, TEXT("STATIC"), NULL, 0, 0, 0, 0, 0, HWND_DESKTOP, NULL, GetModuleHandle(NULL), 0);
		if (hWnd == NULL)
			throw std::runtime_error("Can't create window");
		oldproc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG>(&myproc));
	}

	~Repeater() {
		DestroyWindow(hWnd);
	}

	void play() {
		Player player(fn.c_str());
		std::lock_guard<std::recursive_mutex> lock(devmap_mutex);
		player.play(hWnd);
		devmap.insert(decltype(devmap)::value_type(player.device(), std::move(player)));
	}

	HWND wnd() const { return hWnd; }

	void stop() {
		std::lock_guard<std::recursive_mutex> lock(devmap_mutex);
		devmap.clear();
	}


private:
	HWND hWnd;
	tstring fn;
	std::recursive_mutex devmap_mutex;
	std::map<MCIDEVICEID, Player> devmap;
	WNDPROC oldproc;

	static LRESULT CALLBACK myproc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		auto self = reinterpret_cast<Repeater*>(GetWindowLong(hWnd, GWLP_USERDATA));
		switch (Msg) {
		case MM_MCINOTIFY: {
			// see https://msdn.microsoft.com/ru-ru/library/windows/desktop/dd757358(v=vs.85).aspx
			std::lock_guard<std::recursive_mutex> lock(self->devmap_mutex);
			self->devmap.erase(static_cast<MCIDEVICEID>(lParam));
			return 0;
		}
		case WM_DESTROY: {
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG>(self->oldproc));
		}
		default:
			break;
		}
		return CallWindowProc(self->oldproc, hWnd, Msg, wParam, lParam);
	}

};

int main() {
	// USE MP3. Forget about WAV.
	LPCTSTR filename = TEXT("shoot.mp3");

#if 0
	// without notifications
	Player dev1(filename), dev2(filename);
	dev1.play();
	Sleep(1000);
	dev2.play();
	Sleep(10000);
#else
	// with notifications
	{
		Repeater rep(filename);

		std::thread thread([&rep] {
			for (int i = 0; i < 5; ++i) {
				rep.play();
				Sleep(1000);
			}
			Sleep(1000);
			rep.stop(); // .stop() MUST be called from the same thread as ALL .play() !!!
			PostMessage(rep.wnd(), WM_QUIT, 0, 0); // interrupt message processing queue
		});

		thread.join();
	}
	Sleep(10000); // silence
#endif

	return 0;
}