// Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <cstdlib>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include <cctype>
#include <conio.h>

#include <map>
#include <thread>

#include "Graphic.h"
#include "Menu.h"
#include "Sound.h"
#include "interface.h"
#include "protocol.h"
#include "Player.h"

#include "Skills/Shoot.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

using namespace std;

extern Graphic graphic;
extern Sound sound;

DWORD my_id = NULL;

// 서버 응답 수신 후 반영
void receive(SOCKET sock)
{
	char buff[1200] = "";

	map<DWORD, Player*> players;

	while (1)
	{
		int len = recv(sock, buff, 1200, NULL);
		if (len <= 0)
		{
			my_id = NULL;
			for (map<DWORD, Player*>::iterator iter = players.begin(); iter != players.end(); iter++)
				delete iter->second;

			graphic.stop();
			return;
		}

		PDUHello* pdu_hello;
		PDUMov* pdu_mov;
		PDUCastSkill* pdu_cast_skill;
		PDUHit* pdu_hit;
		PDUDie* pdu_die;

		Player* player;

		/*
		 * 버그수정 : 간혹 타이밍이 안좋은 경우에
		 * recv할 때 서버로부터 응답을 여러개씩 몰아받게 되는데,
		 * 그럴때 마다 첫 응답만 처리하고 그 뒤의 응답은 그대로 무시해서
		 * 화면이 깨지는 버그가 있었음.
		 * 
		 * 처리할 데이터 양(len - complete_len)이 0이 될 때까지
		 * 데이터 처리를 반복하게 수정함.
		 */
		int complete_len = 0;
		while (len - complete_len)
		{
			switch ((buff + complete_len)[0])
			{
			case HELLO:
				// 버그수정 : <PDUHello*>(buff) => <PDUHello*>(buff + complete_len).
				pdu_hello = reinterpret_cast<PDUHello*>(buff + complete_len);
				
				if (!my_id) // 첫빠다로 받은 hello 패킷이라면
					my_id = pdu_hello->id; // 이 패킷의 id값은 나의 아이디
				else
				{
					players[pdu_hello->id] = new Player(pdu_hello->pos, pdu_hello->HP, pdu_hello->chracter,
						my_id == pdu_hello->id, pdu_hello->len_skills, pdu_hello->skills);

				}
				complete_len += sizeof(PDUHello);
				break;
		
			case MOV:
				pdu_mov = reinterpret_cast<PDUMov*>(buff + complete_len);
				player = players[pdu_mov->id];
				// 버그수정 : player 객체 유효성 검사
				if (player)
					player->move(pdu_mov->dir);
				complete_len += sizeof(PDUMov);
				break;

			case CAST_SKILL:
				pdu_cast_skill = reinterpret_cast<PDUCastSkill*>(buff + complete_len);
				player = players[pdu_cast_skill->id];
				if (player)
					player->cast_skill(pdu_cast_skill->skill_type, pdu_cast_skill->dir);
				complete_len += sizeof(PDUCastSkill);
				break;

			case HIT:
				pdu_hit = reinterpret_cast<PDUHit*>(buff + complete_len);
				player = players[pdu_hit->attacker_id];
				if (player)
					player->attack(players[pdu_hit->victim_id], pdu_hit->skill_type);
				complete_len += sizeof(PDUHit);
				break;

			case DIE:
				pdu_die = reinterpret_cast<PDUDie*>(buff + complete_len);
				player = players[pdu_die->id];
				if (player)
					delete players[pdu_die->id];
				players.erase(pdu_die->id);
				complete_len += sizeof(PDUDie);
				break;
			}
		}
	}
}

Menu active_skill_menu(COORD{ FIELD.Left + 2 * FIELD_WIDTH + 7, FIELD.Top + 7 },
	"액티브 스킬", true);
SKILL_TYPE active_skills[MAX_ACTIVE_SKILL];

// 불연속적 유저 입력(메뉴 선택, 스킬 선택 등) 처리
void send_discontinual_request(SOCKET sock)
{
	active_skill_menu.appear();

	active_skill_menu.insert_menu("Lv.1 사격");
	active_skill_menu.update_menu(
		active_skill_menu.get_num_contents() - 1,
		0, "Lv.1", DARK_YELLOW);
	active_skills[0] = SHOOT;

	active_skill_menu.insert_menu("Lv.1 수련기사의 일격");
	active_skill_menu.update_menu(
		active_skill_menu.get_num_contents() - 1,
		0, "Lv.1", DARK_YELLOW);
	active_skills[1] = SLASH;
	
	active_skill_menu.insert_menu("Lv.1 광선검·참격");
	active_skill_menu.update_menu(
		active_skill_menu.get_num_contents() - 1,
		0, "Lv.1", DARK_YELLOW);
	active_skills[2] = LIGHTSABER_SLASH;

	active_skill_menu.insert_menu("Lv.1 양수검·참격");
	active_skill_menu.update_menu(
		active_skill_menu.get_num_contents() - 1,
		0, "Lv.1", DARK_YELLOW);
	active_skills[3] = ZWEIHANDER_SLASH;

	active_skill_menu.insert_menu("Lv.1 풍마참(風磨斬)");
	active_skill_menu.update_menu(
		active_skill_menu.get_num_contents() - 1,
		0, "Lv.1", DARK_YELLOW);
	active_skills[4] = WIND_SLASH;

	while (graphic.is_started()) // 게임이 진행되는 도중에 반복
	{
		char user_input = toupper(_getch());
		
		switch (user_input)
		{
		case 'R':
			// 액티브 스킬 메뉴의 커서를 밑으로 내림
			if (!active_skill_menu.set_focus(active_skill_menu.get_focus() + 1))
				// 커서가 맨 끝이라면 커서는 처음으로 돌아감
				active_skill_menu.set_focus(0);
			break;
		}
	}

	active_skill_menu.disappear();
	active_skill_menu.clear();
}

// 연속적 유저 입력(움직임, 스킬 사용 등) 처리
void send_continual_request(SOCKET sock)
{
	unsigned char inputs[] = { 'W', 'A', 'S', 'D', VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, NULL };

	HWND console_wnd = GetForegroundWindow();

	while (graphic.is_started()) // 게임이 진행되는 중에 반복
	{
		for (int i = 0; inputs[i] && console_wnd == GetForegroundWindow(); i++)
			// 키가 눌려있는 상태라면
			if (GetKeyState(inputs[i]) & 0x8000)
			{
				PDUMov pdu_mov;
				PDUCastSkill pdu_cast_skill;

				char* buff = NULL;
				int len = 0;

				switch (inputs[i])
				{
				case 'W': pdu_mov.dir = UP;	break;
				case 'A': pdu_mov.dir = LEFT; break;
				case 'S': pdu_mov.dir = DOWN; break;
				case 'D': pdu_mov.dir = RIGHT; break;

				case VK_UP: pdu_cast_skill.dir = UP; break;
				case VK_DOWN: pdu_cast_skill.dir = DOWN; break;
				case VK_LEFT: pdu_cast_skill.dir = LEFT; break;
				case VK_RIGHT: pdu_cast_skill.dir = RIGHT; break;
				}

				switch (inputs[i])
				{
				case 'W': case 'A': case 'S': case 'D': // 무빙
					len = sizeof(PDUMov);
					buff = (char*)&pdu_mov;
					break;
				case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT: // 스킬 사용
					// 사용 스킬 종류는 메뉴에서 선택된 스킬
					pdu_cast_skill.skill_type = active_skills[active_skill_menu.get_focus()];
					len = sizeof(PDUCastSkill);
					buff = (char*)&pdu_cast_skill;
					break;
				}

				send(sock, buff, len, NULL);
			}

		Sleep(30);
	}

}

int main()
{
	SetConsoleTitleA("Terminal War");


	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << " WSAStartup error : " << WSAGetLastError() << endl;
		_getch();
		return 1;
	}

	while (1)
	{
		// 화면 지우기
		{
			HANDLE console_buffer = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD coordScreen = { 0, 0 };
			DWORD cCharsWritten;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			DWORD dwConSize;

			GetConsoleScreenBufferInfo(console_buffer, &csbi);
			dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
			FillConsoleOutputCharacter(console_buffer, ' ', dwConSize, coordScreen, &cCharsWritten);
			SetConsoleCursorPosition(console_buffer, coordScreen);
		}
		cout << endl;

		// 입력버퍼 비우기
		while (_kbhit())
			_getch();

		SOCKET sock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (sock == INVALID_SOCKET)
		{
			cerr << " socket error : " << WSAGetLastError() << endl;
			_getch();
			return 1;
		}
		unsigned long mode = 0; // 0은 블로킹 모드, 1은 논블로킹 모드
		int result = ioctlsocket(sock, FIONBIO, &mode);
		if (result != NO_ERROR)
		{
			cerr << " ioctlsocket error : " << WSAGetLastError() << endl;
			_getch();
			return 1;
		}

		SOCKADDR_IN recvAddr;
		memset(&recvAddr, 0, sizeof(recvAddr));
		recvAddr.sin_family = AF_INET;
		recvAddr.sin_port = htons(PORT);

		int bRet;
		do
		{
			char IP[20] = "";
			cout << " 서버 IP : ";	cin >> IP;
			inet_pton(AF_INET, IP, &(recvAddr.sin_addr.s_addr));

			bRet = connect(sock, (SOCKADDR*)&recvAddr, sizeof(recvAddr));
			if (bRet == SOCKET_ERROR)
			{
				cerr << " connect error : " << WSAGetLastError() << endl;
				_getch();
				system("cls");
				cout << endl;
			}
		} while (bRet);

		// 게임 진행 장면 송출 시작
		graphic.clear_frame();
		graphic.draw_field();
		graphic.start();

		thread(receive, sock).detach();

		// 영문 문자로 캐릭터 선택
		PDUHello pdu_hello;
		pdu_hello.chracter = NULL;
		while (pdu_hello.chracter < 0x41 || (0x5a < pdu_hello.chracter && pdu_hello.chracter < 0x61) || 0x7a < pdu_hello.chracter)
			pdu_hello.chracter = _getch();

		// 게임 유저들에게 나의 존재를 알림
		send(sock, reinterpret_cast<const char*>(&pdu_hello), sizeof(PDUHello), NULL);

		thread(send_discontinual_request, sock).detach();
		send_continual_request(sock);
	}
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
