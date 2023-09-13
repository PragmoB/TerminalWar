// Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <WinSock2.h>
#include <iostream>
#include <ctime>

#include <vector>
#include <thread>

#include "Client.h"
#include "protocol.h"

using namespace std;

void worker(HANDLE completion_port)
{
	while (true) {
		DWORD bytesTransferred;
		ULONG_PTR key;
		LPOVERLAPPED overlapped;

		BOOL result = GetQueuedCompletionStatus(completion_port, &bytesTransferred, &key, &overlapped, INFINITE);

		if (!result || bytesTransferred == 0)
		{
			// 오류 처리
			cerr << "recv error" << endl;
			continue;
		}

		// 클라이언트 데이터 처리
		Client* client = reinterpret_cast<Client*>(overlapped);

		switch (client->context.dataBuffer.buf[0])
		{
		case MOV:
			{
				PDUMov* pdu = 
					reinterpret_cast<PDUMov*>(client->context.dataBuffer.buf);
				client->move(pdu->dir);
				break;
			}

		case SHOOT:
			{
				PDUShoot* pdu = 
					reinterpret_cast<PDUShoot*>(client->context.dataBuffer.buf);
				client->shoot(pdu->dir);
				break;

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
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Failed to initialize Winsock." << std::endl;
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		std::cerr << "Failed to create listen socket." << std::endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	if (::bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cerr << "Failed to bind the socket." << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Failed to listen on the socket." << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (completionPort == NULL) {
		std::cerr << "Failed to create completion port." << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// IOCP 작업자 스레드를 생성
	for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
		thread(worker, completionPort);
	}

	std::cout << "IOCP server is running..." << std::endl;

	while (true) {
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			std::cerr << "Failed to accept client connection." << std::endl;
			continue;
		}

		Client* client = new Client(POINT{ 5, 5 }, 'O');
		Client::push(client);

		client->context.socket = clientSocket;
		client->context.dataBuffer.buf = client->context.buffer;
		client->context.dataBuffer.len = 1024;

		client->hello();

		// 클라이언트 소켓을 완료 포트에 연결
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), completionPort, reinterpret_cast<ULONG_PTR>(client), 0);

		// 비동기 데이터 수신 시작
		DWORD flags = 0;
		if (WSARecv(clientSocket, &client->context.dataBuffer, 1, NULL, &flags, &client->context.overlapped, NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				// 오류 처리
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
