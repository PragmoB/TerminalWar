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
#include "protocol.h"
#include "Player.h"

using namespace std;

extern Graphic graphic;

void receive(SOCKET sock)
{
	/*
	WSAEVENT event = WSACreateEvent();
	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = event;
	WSABUF data_buf;
	int recv_bytes = 0;
	int flags = 0;
	char buff[1000];
	data_buf.buf = buff;
	data_buf.len = sizeof(buff);*/
	char buff[1000] = "";
	

	static map<uint32_t, Player*> players;

	while (1)
	{
		/*
		if (WSARecv(sock, &data_buf, 1, (LPDWORD)&recv_bytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				graphic.stop();
				system("cls");
				cerr << endl;
				cerr << " 연결에 오류가 생겼습니다" << endl;
				_getch();
				exit(1);
			}
		}*/
		if (recv(sock, buff, 1000, NULL) == SOCKET_ERROR)
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

		switch (buff[0])
		{
		case HELLO:
			pdu_hello = reinterpret_cast<PDUHello*>(buff);
			players[pdu_hello->id] = new Player(pdu_hello->pos, pdu_hello->chracter);
			break;
		
		case MOV:
			pdu_mov = reinterpret_cast<PDUMov*>(buff);
			players[pdu_mov->id]->move(pdu_mov->dir);
			break;

		case SHOOT:
			pdu_shoot = reinterpret_cast<PDUShoot*>(buff);
			players[pdu_shoot->id]->shoot(pdu_shoot->dir);
			break;

		case HIT:
			pdu_hit = reinterpret_cast<PDUHit*>(buff);
			players[pdu_hit->id]->hit();
			break;

		case DIE:
			pdu_die = reinterpret_cast<PDUDie*>(buff);
			delete players[pdu_die->id];
			players.erase(pdu_die->id);
			break;
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
		}
	} while (bRet);

	PDUHello pdu_hello;
	cout << " 캐릭터 선택 : ";	pdu_hello.chracter = _getch();

	thread(receive, sock).detach();

	graphic.clear_frame();
	graphic.draw_field();
	graphic.start();

	/*
	WSAEVENT event = WSACreateEvent();
	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = event;
	WSABUF data_buf;
	data_buf.len = sizeof(PDUHello);
	data_buf.buf = reinterpret_cast<char*>(&pdu_hello);
	int send_bytes = 0;

	WSASend(sock, &data_buf, 1, (LPDWORD)&send_bytes, 0, &overlapped, NULL);*/

	send(sock, reinterpret_cast<const char*>(&pdu_hello), sizeof(PDUHello), NULL);
	
	while (1)
	{
		PDUMov pdu_mov;
		PDUShoot pdu_shoot;

		/*
		data_buf.len = 0;
		data_buf.buf = NULL;
		*/
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
			//WSASend(sock, &data_buf, 1, (LPDWORD)&send_bytes, 0, &overlapped, NULL);
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
