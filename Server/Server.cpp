﻿// Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <WinSock2.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#include <vector>
#include <thread>

#include "Player.h"
#include "Background.h"
#include "protocol.h"

#include "Values/interface.h"

using namespace std;

extern Background background;

void worker(HANDLE completion_port)
{
	while (1)
	{
		DWORD len;
		ULONG_PTR key;
		LPOVERLAPPED overlapped;

		BOOL bSuccess = GetQueuedCompletionStatus(completion_port, &len, &key, &overlapped, INFINITE);

		// 클라이언트 데이터 처리
		Player* player = reinterpret_cast<Player*>(overlapped);
		
		if (len == 0)
		{
			cout << " Player " << player->context.socket << " died" << endl;
			closesocket(player->context.socket);

			// 클라이언트 리스트에서 player 삭제
			for (list<Player*>::iterator iter = background.players.begin();
				iter != background.players.end(); iter++)
				if (*iter == player)
				{
					delete player;
					background.players.erase(iter);
					break;
				}
			
			continue;
		}

		union
		{
			PDUHello* pdu_hello;
			PDUMovReq* pdu_mov_req;
			PDUCastSkill* pdu_cast_skill;
			PDUUpgradeSkill* pdu_upgrade_skill;
		};

		int complete_len = 0;

		while ((signed)len - complete_len > 0)
		{
			switch ((player->context.dataBuffer.buf + complete_len)[0])
			{
			case HELLO:
				pdu_hello = reinterpret_cast<PDUHello*>(player->context.dataBuffer.buf + complete_len);
				player->hello(pdu_hello->chracter);

				complete_len += sizeof(PDUHello);
				break;

			case UPGRADE_SKILL:
				pdu_upgrade_skill = reinterpret_cast<PDUUpgradeSkill*>(player->context.dataBuffer.buf + complete_len);
				player->upgrade_skill(pdu_upgrade_skill->skill_type, pdu_upgrade_skill->upgraded_skill_type);
				
				complete_len += sizeof(PDUUpgradeSkill);
				break;
			default:
				complete_len = len;
			}
		}

		// 다음 IO 작업을 시작
		DWORD flags = 0;
		WSARecv(player->context.socket, &player->context.dataBuffer, 
			1, NULL, &flags, &player->context.overlapped, NULL);
	}
}
void workerUDP(SOCKET sock)
{
	while (1)
	{
		sockaddr_in peer_addr;
		int peer_addr_len = sizeof(peer_addr);
		char buff[2000] = "";
		recvfrom(sock, buff, sizeof(buff), NULL, (struct sockaddr*)&peer_addr, &peer_addr_len);
		union
		{
			PDUUDPHello* pdu_udp_hello;
			PDUMovReq* pdu_mov_req;
			PDUCastSkill* pdu_cast_skill;
		};
		pdu_mov_req = reinterpret_cast<PDUMovReq*>(buff);

		// 요청 클라이언트의 player 객체 결정하기
		Player* player = NULL;
		DWORD player_id = NULL;
		switch (buff[0])
		{
		case UDP_HELLO: player_id = pdu_udp_hello->id;
		case MOV: player_id = pdu_mov_req->id;
		case CAST_SKILL: player_id = pdu_cast_skill->id;
		}
		for (list<Player*>::iterator iter = background.players.begin();
			iter != background.players.end();
			iter++)
		{
			player = *iter;
			if (player->addr.sin_addr.S_un.S_addr == peer_addr.sin_addr.S_un.S_addr &&
				player->context.socket == player_id)
			{
				// 클라이언트 측 UDP 포트 초기화
				player->addr = peer_addr;
				break;
			}
			else
				player = NULL;
		}

		switch (buff[0])
		{
		case MOV:
			pdu_mov_req = reinterpret_cast<PDUMovReq*>(buff);
			if (player)
				player->move(pdu_mov_req->dir);
			break;
		case CAST_SKILL:
			pdu_cast_skill = reinterpret_cast<PDUCastSkill*>(buff);
			if (player)
				player->cast_skill(pdu_cast_skill->skill_type, pdu_cast_skill->dir);
		default:
			break;
		}

	}
}
int main()
{
	srand(time(NULL));
	cout << endl;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << " Failed to initialize Winsock." << std::endl;
		_getch();
		return 1;
	}
	cout << " Succeed to initialize Winsock." << endl;

	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET)
	{
		cerr << " Failed to create UDP socket." << endl;
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to create UDP socket." << endl;


	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		std::cerr << " Failed to create listen socket." << std::endl;
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to create listen socket." << endl;

	int port;
	cout << " Port : ";	cin >> port;
	sockaddr_in udpServerAddress;
	udpServerAddress.sin_family  = AF_INET;
	udpServerAddress.sin_addr.s_addr = INADDR_ANY;
	udpServerAddress.sin_port = htons(port);
	if (::bind(udpSocket, (struct sockaddr*)&udpServerAddress, sizeof(udpServerAddress)) == SOCKET_ERROR)
	{
		cerr << " Failed to bind UDP socket." << endl;
		closesocket(udpSocket);
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to bind UDP socket." << endl;
	Player::udp_socket = udpSocket;

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	if (::bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cerr << " Failed to bind the socket." << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to bind the socket.." << endl;

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << " Failed to listen on the socket." << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to listen on the socket.." << endl;

	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (completionPort == NULL) {
		std::cerr << " Failed to create completion port." << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to create completion port." << endl;

	// IOCP 작업자 스레드를 생성
	
	for (int i = 0; i < 2; i++)
		thread(worker, completionPort).detach();
	for (int i = 0; i < 4; i++)
		thread(workerUDP, udpSocket).detach();

	std::cout << " IOCP server is running..." << std::endl << std::endl;

	while (true) {
		// 최대 동접자 9명으로 제한
		while (background.players.size() >= 9)
			Sleep(1000);

		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);

		SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&client_addr, &client_addr_size);

		if (clientSocket == INVALID_SOCKET) {
			std::cerr << " Failed to accept client connection." << std::endl;
			continue;
		}
		cout << " Player " << clientSocket << " accepted" << endl;

		// 클라이언트 생성 후 리스트에 등록
		ClientContext client_context;
		client_context.socket = clientSocket;
		client_context.dataBuffer.len = 1024;
		client_context.overlapped.hEvent = NULL;
		Player* player = new Player(client_context, client_addr,
									COORD{ (SHORT)(rand() % FIELD_WIDTH + 1), (SHORT)(rand() % FIELD_HEIGHT + 1) });
		background.players.push_back(player);

		// 클라이언트 소켓을 완료 포트에 연결
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), completionPort, reinterpret_cast<ULONG_PTR>(player), 0);

		// 비동기 데이터 수신 시작
		DWORD flags = 0;
		if (WSARecv(clientSocket, &player->context.dataBuffer, 1, NULL, &flags, &player->context.overlapped, NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				// 오류 처리
				std::cerr << " Failed to recv client : " << WSAGetLastError() << std::endl;
				background.players.pop_back();
				delete player;
			}
		}
	}

	// 서버 종료 시 정리 작업 수행
	closesocket(listenSocket);
	CloseHandle(completionPort);
	WSACleanup();

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
