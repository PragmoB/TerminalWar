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
#include "Sound.h"
#include "protocol.h"
#include "Player.h"

using namespace std;

extern Graphic graphic;
extern Sound sound;

void receive(SOCKET sock)
{
	char buff[1000] = "";

	static map<uint32_t, Player*> players;

	while (1)
	{
		int len = recv(sock, buff, 1000, NULL);
		if (len <= 0)
		{
			graphic.stop();
			system("cls");
			cerr << endl;
			cerr << " 연결에 오류가 생겼습니다" << endl;
			_getch();
			exit(1);
		}

		PDUHello* pdu_hello;
		PDUMov* pdu_mov;
		PDUShoot* pdu_shoot;
		PDUHit* pdu_hit;
		PDUDie* pdu_die;

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
				pdu_hello = reinterpret_cast<PDUHello*>(buff + complete_len);	// 이거 찾는데 얼마나 qudtls같이 헛고생을 많이 했는지 당사자 아닌사람은 모를거다ㅠㅠ 별거 아닌거 같이 보여도 실시간 온라인게임이라 디버깅하기가 진짜 어렵다.
				players[pdu_hello->id] = new Player(pdu_hello->pos, pdu_hello->chracter);
				complete_len += sizeof(PDUHello);
				break;
		
			case MOV:
				pdu_mov = reinterpret_cast<PDUMov*>(buff + complete_len);
				players[pdu_mov->id]->move(pdu_mov->dir);
				complete_len += sizeof(PDUMov);
				break;

			case SHOOT:
				pdu_shoot = reinterpret_cast<PDUShoot*>(buff + complete_len);
				players[pdu_shoot->id]->shoot(pdu_shoot->dir);
				complete_len += sizeof(PDUShoot);
				break;

			case HIT:
				pdu_hit = reinterpret_cast<PDUHit*>(buff + complete_len);
				players[pdu_hit->id]->hit();
				complete_len += sizeof(PDUHit);
				break;

			case DIE:
				pdu_die = reinterpret_cast<PDUDie*>(buff + complete_len);
				delete players[pdu_die->id];
				players.erase(pdu_die->id);
				complete_len += sizeof(PDUDie);
				break;
			}
		}
	}
}

int main()
{
	system("title Terminal War");

	cout << endl;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << " WSAStartup error : " << WSAGetLastError() << endl;
		_getch();
		return 1;
	}

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

	PDUHello pdu_hello;
	pdu_hello.chracter = NULL;
	cout << " 영어 알파벳 문자로 캐릭터 선택 : ";	
	while (pdu_hello.chracter < 0x41 || (0x5a < pdu_hello.chracter && pdu_hello.chracter < 0x61) || 0x7a < pdu_hello.chracter)
		pdu_hello.chracter = _getch();

	thread(receive, sock).detach();

	graphic.clear_frame();
	graphic.draw_field();
	graphic.start();

	send(sock, reinterpret_cast<const char*>(&pdu_hello), sizeof(PDUHello), NULL);
	
	// 입력 전 버퍼 비우기
	while (_kbhit())
		_getch();

	while (1)
	{
		PDUMov pdu_mov;
		PDUShoot pdu_shoot;

		char* buff = NULL;
		int len = 0;

		unsigned char input = toupper(_getch());

		// 움직임
		{
			switch (input)
			{
			case 'W':
				pdu_mov.dir = UP;
				break;
			case 'A':
				pdu_mov.dir = LEFT;
				break;
			case 'S':
				pdu_mov.dir = DOWN;
				break;
			case 'D':
				pdu_mov.dir = RIGHT;
				break;
			}
			if (pdu_mov.dir)
			{
				len = sizeof(PDUMov);
				buff = reinterpret_cast<char*>(&pdu_mov);
			}
		}
		// 총 쏘기
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case 72 :
				pdu_shoot.dir = UP;
				break;

			case 80 :
				pdu_shoot.dir = DOWN;
				break;

			case 75 :
				pdu_shoot.dir = LEFT;
				break;

			case 77 :
				pdu_shoot.dir = RIGHT;
				break;
			}
			len = sizeof(PDUShoot);
			buff = reinterpret_cast<char*>(&pdu_shoot);
		}

		// 무엇이든 유의미한 이벤트가 있었다면
		if (len)
		{
			// 전송
			send(sock, buff, len, NULL);
		}
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
