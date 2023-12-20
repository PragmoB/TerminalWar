// Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <cstdlib>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include <cctype>
#include <conio.h>

#include <list>
#include <map>
#include <thread>

#include "Graphic.h"
#include "Menu.h"
#include "Sound.h"
#include "interface.h"
#include "protocol.h"
#include "Player.h"
#include "EnergyBar.h"
#include "UpgradeOptionList.h"

#include "Items/Item.h"
#include "Items/Heart.h"
#include "Items/Energy.h"

#include "Skills/Skill.h"
#include "Skills/Shoot.h"
#include "Skills/Slash.h"
#include "Skills/LightsaberSlash.h"
#include "Skills/ZweihanderSlash.h"
#include "Skills/WindSlash.h"

using namespace std;

extern Graphic graphic;
extern Sound sound;

extern map<DWORD, Player*> players;
extern list<Item*> items;

DWORD my_id = NULL;

UpgradeOptionList* upgrade_option_list;
UpgradeOptionList* evolution_option_list;

static union
{
	SKILL_TYPE upgrade_active_skill_options[NUM_UPGRADE_SKILL_OPTIONS + 1];
};
static union
{
	SKILL_TYPE evolution_active_skill_options[10];
};

SKILL_TYPE upgrade_skill_type;
int len_upgrade_skill_options = 0;
int len_evolution_skill_options = 0;
bool upgradable = false;
int focus_is_on_evolution_option_list = false;

EnergyBar energy_bar;

Menu active_skill_menu(COORD{ FIELD.Left + 2 * FIELD_WIDTH + 7, FIELD.Top + 2 },
	"액티브 스킬", true);
SKILL_TYPE active_skills[MAX_ACTIVE_SKILL];

// 서버 응답 수신 후 반영
void receive(SOCKET sock)
{
	char buff[4000] = "";

	while (1)
	{
		int len = recv(sock, buff, 4000, NULL);
		if (len <= 0)
		{
			for (map<DWORD, Player*>::iterator iter = players.begin(); iter != players.end();)
			{
				delete iter->second;
				players.erase(iter++);
			}
			for (list<Item*>::iterator iter = items.begin(); iter != items.end();)
			{
				(*iter)->disappear();
				delete (*iter);
				items.erase(iter++);
			}

			my_id = NULL;
			energy_bar = EnergyBar();
			graphic.stop();
			return;
		}

		union
		{
			PDUHello* pdu_hello;
			PDUItemInfo* pdu_item_info;
			PDUEarnItem* pdu_earn_item;
			PDUCastSkill* pdu_cast_skill;
			PDUHit* pdu_hit;
			PDUUpgradeSkillOptionInfo* pdu_upgrade_skill_option_info;
			PDUUpgradeSkill* pdu_upgrade_skill;
			PDUDie* pdu_die;
		};

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
				{
					my_id = pdu_hello->id; // 이 패킷의 id값은 나의 아이디
					sound.request(HELLO); // 배경음악 재생
					graphic.draw_field();
				}
				else
				{
					players[pdu_hello->id] = new Player(pdu_hello->pos, pdu_hello->HP, pdu_hello->chracter,
						my_id == pdu_hello->id, pdu_hello->len_skills, pdu_hello->skills, pdu_hello->skill_levels);
					energy_bar.appear();
				}
				complete_len += sizeof(PDUHello);
				break;
			case ITEM_INFO:
			{
				pdu_item_info = reinterpret_cast<PDUItemInfo*>(buff + complete_len);
				Item* item = NULL;

				// 아이템 종류에 따라 아이템 객체 생성
				switch (pdu_item_info->item_type)
				{
				case HEART:  item = new Heart(pdu_item_info->pos); break;
				case ENERGY: item = new Energy(pdu_item_info->level, pdu_item_info->pos); break;
				}
				
				if (item)
				{
					item->appear();
					items.push_back(item);
				}
				complete_len += sizeof(PDUItemInfo);
				break;
			}
			case EARN_ITEM:
				pdu_earn_item = reinterpret_cast<PDUEarnItem*>(buff + complete_len);
				player = players[pdu_earn_item->id];

				{
					// 이번 아이템 획득 이벤트에서 얻은 나의 총 경험치량
					int my_energy_gain = 0;

					// items(아이템 목록)을 돌며 일치하는 아이템을 찾음
					for (list<Item*>::iterator iter = items.begin();
						iter != items.end();)
					{
						Item* item = *iter;
						COORD pos = COORD{ NULL, NULL };
					
						if (item)
							pos = item->get_pos();

						// 발견했으면
						if (pdu_earn_item->pos.X == pos.X && pdu_earn_item->pos.Y == pos.Y)
						{
							Energy* energy_item = dynamic_cast<Energy*>(item);

							// item이 경험치 구슬이었고 먹은게 나 이고 스킬 강화 선택이 대기중이 아니라면
							if (energy_item && pdu_earn_item->id == my_id && !upgradable)
								my_energy_gain += energy_item->get_amount(); // 에너지 표시줄에 반영

							items.erase(iter++);

							// 해당하는 플레이어가 흡수
							if(player)
								player->earn_item(item);
						}
						else
							iter++;
					}

					energy_bar.earn_energy(my_energy_gain);
				}
				complete_len += sizeof(PDUEarnItem);
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

			case UPGRADE_SKILL_OPTION_INFO:
			{
				sound.request(UPGRADE_SKILL_OPTION_INFO);
				pdu_upgrade_skill_option_info = reinterpret_cast<PDUUpgradeSkillOptionInfo*>(buff + complete_len);
				upgrade_option_list = new UpgradeOptionList(COORD{ FIELD.Left + 1, FIELD.Top - 6 }, "레벨업! 강화할 스킬을 선택하세요");
				upgrade_option_list->appear();

				int i;
				char content[200] = "";

				for (i = 0; i < NUM_UPGRADE_SKILL_OPTIONS &&
					pdu_upgrade_skill_option_info->active_skill_types[i] != SKILL_TYPE::UNKNOWN; i++)
				{
					// 스킬 강화 옵션 등록
					upgrade_active_skill_options[i] = pdu_upgrade_skill_option_info->active_skill_types[i];

					// 스킬 강화 메시지(content) 구성후 강화 옵션 리스트에 출력
					player = players[my_id];
					Skill* skill = player->get_active_skill(pdu_upgrade_skill_option_info->active_skill_types[i]);
					if (skill)
						skill->get_level_up_message(content, sizeof(content));
					else
						Skill::get_object_by_type(pdu_upgrade_skill_option_info->active_skill_types[i])->get_learn_message(content, sizeof(content));

					upgrade_option_list->push_content(content);
				}
				upgrade_active_skill_options[i] = SKILL_TYPE::UNKNOWN;
				len_upgrade_skill_options = i;
				sprintf_s(content, "선택 포기, 경험치 %d%% 환수", ENERGY_REFUND_RATE);
				upgrade_option_list->push_content(content);

				upgradable = true;
				complete_len += sizeof(PDUUpgradeSkillOptionInfo);
				break;
			}
			case UPGRADE_SKILL:
				pdu_upgrade_skill = reinterpret_cast<PDUUpgradeSkill*>(buff + complete_len);
				player = players[pdu_upgrade_skill->id];
				if (player)
				{
					player->upgrade_skill(pdu_upgrade_skill->active_skill_type, pdu_upgrade_skill->upgraded_active_skill_type);
					
					// 업그레이드 대상이 나 일경우 액티브 스킬 목록 업데이트
					if (pdu_upgrade_skill->id == my_id)
					{
						int i;
						for (i = 0; i < MAX_ACTIVE_SKILL && active_skills[i] != SKILL_TYPE::UNKNOWN; i++)
						{
							if (active_skills[i] == pdu_upgrade_skill->active_skill_type)
								break;
						}
						
						Skill* skill = player->get_active_skill(pdu_upgrade_skill->active_skill_type);
						char buff[100] = "";
						if (skill)
						{

							// 진화 가능 표시
							if (skill->get_level() >= skill->get_max_level())
							{
								sprintf_s(buff, "Lv.%2d", skill->get_level());
								active_skill_menu.update_menu(i, 0, buff, GREEN);
							}
							// 새로운 스킬이 추가됨
							else if (skill->get_level() == 1)
							{
								sprintf_s(buff, "Lv.%2d %s", skill->get_level(), skill->get_skill_name());
								active_skills[active_skill_menu.get_num_contents()] = pdu_upgrade_skill->active_skill_type;
								active_skill_menu.insert_menu(buff);

								sprintf_s(buff, "Lv.%2d", skill->get_level());
								active_skill_menu.update_menu(active_skill_menu.get_num_contents() - 1, 0, buff, DARK_YELLOW);
							}
							// 레벨업
							else
							{
								sprintf_s(buff, "Lv.%2d", skill->get_level());
								active_skill_menu.update_menu(i, 0, buff, DARK_YELLOW);
							}
						}
						// 진화됨
						else
						{
							skill = player->get_active_skill(pdu_upgrade_skill->upgraded_active_skill_type);
							if (skill)
								for (int i = 0; i < active_skill_menu.get_num_contents(); i++)
								{
									if (Skill::get_object_by_type(active_skills[i])->upgradable_to(skill->get_type()))
									{
										active_skills[i] = skill->get_type();

										sprintf_s(buff, "Lv.%2d %s", skill->get_level(), skill->get_skill_name());
										active_skill_menu.disappear();
										active_skill_menu.update_menu(i, 0, buff);
										sprintf_s(buff, "Lv.%2d", skill->get_level());
										active_skill_menu.update_menu(i, 0, buff, DARK_YELLOW);
										active_skill_menu.appear();
										break;
									}
								}
						}
					}
				}
				complete_len += sizeof(PDUUpgradeSkill);
				break;
			case DIE:
				pdu_die = reinterpret_cast<PDUDie*>(buff + complete_len);
				player = players[pdu_die->id];

				if (pdu_die->attacker_id == my_id && !upgradable)
					energy_bar.hunt_player(pdu_die->level);

				if (player)
					delete players[pdu_die->id];
				players.erase(pdu_die->id);
				complete_len += sizeof(PDUDie);
				break;
			}
		}
	}
}

void receive_udp(SOCKET udp_sock)
{
	while (graphic.is_started())
	{
		char buff[1000];
		recv(udp_sock, buff, sizeof(buff), NULL);

		Player* player = NULL;
		
		union
		{
			PDUMovRes* pdu_mov_res;
		};

		switch (buff[0])
		{
		case MOV:
			pdu_mov_res = reinterpret_cast<PDUMovRes*>(buff);
			player = players[pdu_mov_res->id];
			if (player)
				player->move(pdu_mov_res->pos);
			break;
		default :
			break;
		}
	}
}


// 불연속적 유저 입력(메뉴 선택, 스킬 선택 등) 처리
void send_discontinual_request(SOCKET sock)
{
	active_skill_menu.appear();

	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
		active_skills[i] = SKILL_TYPE::UNKNOWN;

	while (graphic.is_started()) // 게임이 진행되는 도중에 반복
	{
		char user_input = toupper(_getch());
		
		// 스킬 진화 옵션 선택 이벤트
		if ('1' <= user_input && user_input <= ('1' + len_evolution_skill_options) && upgradable && focus_is_on_evolution_option_list)
		{
			focus_is_on_evolution_option_list = false;
			evolution_option_list->disappear();
			delete evolution_option_list;
			len_evolution_skill_options = 0;

			// 뒤로가기
			if (evolution_active_skill_options[user_input - '1'] == SKILL_TYPE::UNKNOWN)
				upgrade_option_list->appear();
			else
			{
				delete upgrade_option_list;
				len_upgrade_skill_options = 0;
				len_evolution_skill_options = 0;
				upgradable = false;
				focus_is_on_evolution_option_list = false;

				PDUUpgradeSkill pdu;
				pdu.skill_is_active = true;
				pdu.active_skill_type = upgrade_skill_type;
				pdu.upgraded_active_skill_type = evolution_active_skill_options[user_input - '1'];
				send(sock, (const char*)&pdu, sizeof(PDUUpgradeSkill), NULL);

			}
		}
		// 스킬 강화 요청 이벤트
		else if ('1' <= user_input && user_input <= ('1' + len_upgrade_skill_options) && upgradable && !focus_is_on_evolution_option_list)
		{
			Player* me = players[my_id];
			if (me)
			{
				upgrade_option_list->disappear();

				PDUUpgradeSkill pdu;
				pdu.skill_is_active = true;
				pdu.active_skill_type = upgrade_active_skill_options[user_input - '1'];
				Skill* my_skill = me->get_active_skill(pdu.active_skill_type);

				if (my_skill)
				{
					// 진화
					if (my_skill->get_level() == my_skill->get_max_level())
					{
						char buff[200] = "";
						sprintf_s(buff, "%s | 진화 옵션을 선택하세요", my_skill->get_skill_name());
						evolution_option_list = new UpgradeOptionList(COORD{ FIELD.Left + 1, FIELD.Top - 6 }, buff);

						// 모든 스킬들 중 진화 옵션으로 유효한 것만 추출하여 리스트에 박음
						for (int i = 0; i < sizeof(SKILL_TYPE_LIST) / sizeof(SKILL_TYPE); i++)
						{
							if (my_skill->upgradable_to(SKILL_TYPE_LIST[i]))
							{
								Skill::get_object_by_type(SKILL_TYPE_LIST[i])->get_learn_message(buff, sizeof(buff));
								evolution_option_list->push_content(buff);
								evolution_active_skill_options[len_evolution_skill_options++] = SKILL_TYPE_LIST[i];
							}
						}
						evolution_option_list->push_content("돌아가기");
						evolution_active_skill_options[len_evolution_skill_options] = SKILL_TYPE::UNKNOWN;

						upgrade_skill_type = pdu.active_skill_type;
						evolution_option_list->appear();
						focus_is_on_evolution_option_list = true;
					}
					// 레벨업
					else
					{
						upgrade_option_list->disappear();
						delete upgrade_option_list;
						len_upgrade_skill_options = 0;
						len_evolution_skill_options = 0;
						upgradable = false;
						focus_is_on_evolution_option_list = false;

						send(sock, (const char*)&pdu, sizeof(PDUUpgradeSkill), NULL);

					}
				}
				// 습득
				else
				{
					upgrade_option_list->disappear();
					delete upgrade_option_list;
					len_upgrade_skill_options = 0;
					len_evolution_skill_options = 0;
					upgradable = false;
					focus_is_on_evolution_option_list = false;

					send(sock, (const char*)&pdu, sizeof(PDUUpgradeSkill), NULL);

					// 포기
					if (pdu.active_skill_type == SKILL_TYPE::UNKNOWN)
						energy_bar.earn_energy(REQUIRED_ENERGY[energy_bar.get_level() - 1] * ENERGY_REFUND_RATE / 100);
				}
			}
		}
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
void send_continual_request(SOCKET udp_sock)
{
	unsigned char inputs[] = { 'W', 'A', 'S', 'D', VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, NULL };

	HWND console_wnd = GetForegroundWindow();

	while (graphic.is_started()) // 게임이 진행되는 중에 반복
	{
		for (int i = 0; inputs[i] && console_wnd == GetForegroundWindow(); i++)
			// 키가 눌려있는 상태라면
			if (GetKeyState(inputs[i]) & 0x8000)
			{
				PDUMovReq pdu_mov_req;
				PDUCastSkill pdu_cast_skill;

				char* buff = NULL;
				int len = 0;

				switch (inputs[i])
				{
				case 'W': pdu_mov_req.dir = UP;	break;
				case 'A': pdu_mov_req.dir = LEFT; break;
				case 'S': pdu_mov_req.dir = DOWN; break;
				case 'D': pdu_mov_req.dir = RIGHT; break;

				case VK_UP: pdu_cast_skill.dir = UP; break;
				case VK_DOWN: pdu_cast_skill.dir = DOWN; break;
				case VK_LEFT: pdu_cast_skill.dir = LEFT; break;
				case VK_RIGHT: pdu_cast_skill.dir = RIGHT; break;
				}

				switch (inputs[i])
				{
				case 'W': case 'A': case 'S': case 'D': // 무빙
					pdu_mov_req.id = my_id;
					len = sizeof(PDUMovReq);
					buff = (char*)&pdu_mov_req;
					break;
				case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT: // 스킬 사용
					// 사용 스킬 종류는 메뉴에서 선택된 스킬
					pdu_cast_skill.id = my_id;
					pdu_cast_skill.skill_type = active_skills[active_skill_menu.get_focus()];
					len = sizeof(PDUCastSkill);
					buff = (char*)&pdu_cast_skill;
					break;
				}

				send(udp_sock, buff, len, NULL);
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
			cerr << " stream socket error : " << WSAGetLastError() << endl;
			_getch();
			return 1;
		}
		SOCKET udpSock = WSASocket(PF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (sock == INVALID_SOCKET)
		{
			cerr << " udp socket error : " << WSAGetLastError() << endl;
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

		SOCKADDR_IN recvAddr, udpAddr;
		memset(&recvAddr, 0, sizeof(recvAddr));
		memset(&udpAddr, 0, sizeof(udpAddr));
		recvAddr.sin_family = udpAddr.sin_family = AF_INET;

		int bRet;
		do
		{
			char IP[20] = "";
			cout << " 서버 IP : ";	cin >> IP;
			inet_pton(AF_INET, IP, &(recvAddr.sin_addr.s_addr));
			inet_pton(AF_INET, IP, &(udpAddr.sin_addr.s_addr));

			int port;
			cout << " 서버 포트 : ";	cin >> port;
			udpAddr.sin_port = htons(port);
			recvAddr.sin_port = htons(port);

			bRet = connect(udpSock, (SOCKADDR*)&udpAddr, sizeof(udpAddr));
			if (bRet == SOCKET_ERROR)
			{
				cerr << " udp connect error : " << WSAGetLastError() << endl;
				_getch();
				system("cls");
				cout << endl;
				continue;
			}
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
		graphic.start();
		graphic.clear_frame();
		graphic.draw_field("동접자가 많아 접속 대기중입니다. 잠시만 기다려주세요. 또는 다른 서버를 이용해주세요.");

		thread(receive, sock).detach();
		thread(receive_udp, udpSock).detach();

		// 서버로부터 클라이언트 id를 할당받을때 까지 대기
		while (!my_id)
			Sleep(10);

		// UDP 포트 알림
		PDUUDPHello pdu_UDP_hello;
		pdu_UDP_hello.id = my_id;
		for (int i = 0; i < 10; i++)
			send(udpSock, (const char*)&pdu_UDP_hello, sizeof(PDUUDPHello), 0);

		// 영문 문자로 캐릭터 선택
		PDUHello pdu_hello;
		pdu_hello.chracter = NULL;
		while (!my_id || pdu_hello.chracter < 0x41 || (0x5a < pdu_hello.chracter && pdu_hello.chracter < 0x61) || 0x7a < pdu_hello.chracter)
			pdu_hello.chracter = _getch();

		// 게임 유저들에게 나의 존재를 알림
		send(sock, reinterpret_cast<const char*>(&pdu_hello), sizeof(PDUHello), NULL);

		thread(send_discontinual_request, sock).detach();
		send_continual_request(udpSock);
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
