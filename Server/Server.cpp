// Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <WinSock2.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#include <vector>
#include <thread>
#include <exception>

#include "Client.h"
#include "Background.h"
#include "protocol.h"
#include "interface.h"

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
		Client* client = reinterpret_cast<Client*>(overlapped);
		
		if (len == 0)
		{
			cout << " Client " << client->context.socket << " died" << endl;
			closesocket(client->context.socket);

			// 클라이언트 리스트에서 client 삭제
			for (list<Client*>::iterator iter = background.clients.begin();
				iter != background.clients.end(); iter++)
				if (*iter == client)
				{
					delete client;
					background.clients.erase(iter);
					break;
				}
			
			continue;
		}

		PDUHello* pdu_hello;
		PDUMov* pdu_mov;
		PDUCastSkill* pdu_cast_skill;
		PDUUpgradeSkill* pdu_upgrade_skill;

		int complete_len = 0;

		while ((signed)len - complete_len > 0)
		{
			switch ((client->context.dataBuffer.buf + complete_len)[0])
			{
			case HELLO:
				pdu_hello = reinterpret_cast<PDUHello*>(client->context.dataBuffer.buf + complete_len);
				client->hello(pdu_hello->chracter);

				complete_len += sizeof(PDUHello);
				break;

			case MOV:
				pdu_mov = reinterpret_cast<PDUMov*>(client->context.dataBuffer.buf + complete_len);
				client->move(pdu_mov->dir);
				
				complete_len += sizeof(PDUMov);
				break;

			case CAST_SKILL:
				pdu_cast_skill = reinterpret_cast<PDUCastSkill*>(client->context.dataBuffer.buf + complete_len);
				client->cast_skill(pdu_cast_skill->skill_type, pdu_cast_skill->dir);

				complete_len += sizeof(PDUCastSkill);
				break;

			case UPGRADE_SKILL:
				pdu_upgrade_skill = reinterpret_cast<PDUUpgradeSkill*>(client->context.dataBuffer.buf + complete_len);
				if (pdu_upgrade_skill->skill_is_active)
					client->upgrade_skill(pdu_upgrade_skill->active_skill_type, pdu_upgrade_skill->upgraded_active_skill_type);
				
				complete_len += sizeof(PDUUpgradeSkill);
				break;
			default:
				complete_len += 400;
			}
		}

		// 다음 IO 작업을 시작
		DWORD flags = 0;
		WSARecv(client->context.socket, &client->context.dataBuffer, 
			1, NULL, &flags, &client->context.overlapped, NULL);
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

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		std::cerr << " Failed to create listen socket." << std::endl;
		WSACleanup();
		_getch();
		return 1;
	}
	cout << " Succeed to create listen socket." << endl;

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	int port;
	cout << " Port : ";	cin >> port;
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
	
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++) {
		thread(worker, completionPort).detach();
	}

	std::cout << " IOCP server is running..." << std::endl << std::endl;

	while (true) {
		// 최대 동접자 9명으로 제한
		while (background.clients.size() >= 9)
			Sleep(1000);

		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			std::cerr << " Failed to accept client connection." << std::endl;
			continue;
		}
		cout << " Client " << clientSocket << " accepted" << endl;

		// 클라이언트 생성 후 리스트에 등록
		ClientContext client_context;
		client_context.socket = clientSocket;
		client_context.dataBuffer.len = 1024;
		client_context.overlapped.hEvent = NULL;
		Client* client = new Client(client_context,
									COORD{ (SHORT)(rand() % FIELD_WIDTH + 1), (SHORT)(rand() % FIELD_HEIGHT + 1) });
		background.clients.push_back(client);

		// 클라이언트 소켓을 완료 포트에 연결
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), completionPort, reinterpret_cast<ULONG_PTR>(client), 0);

		// 비동기 데이터 수신 시작
		DWORD flags = 0;
		if (WSARecv(clientSocket, &client->context.dataBuffer, 1, NULL, &flags, &client->context.overlapped, NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				// 오류 처리
				std::cerr << " Failed to recv client : " << WSAGetLastError() << std::endl;
				background.clients.pop_back();
				delete client;
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
