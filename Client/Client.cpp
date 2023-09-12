// Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <windows.h>
#include <cctype>
#include <conio.h>

#include "graphic.h"
#include "protocol.h"

using namespace std;

int main()
{
	system("title Terminal War");

	thread frame_loader(print_frame);
	thread bullet_renderer[10];
	for (uint32_t i = 0; i < 10; i++)
		bullet_renderer[i] = thread(render_bullet);

	clear_frame();
	draw_field();
	frame_loader.detach();
	
	char chracter = _getch();
	int x = 5, y = 5;
	welcome_user(x, y, chracter);

	while (1)
	{
		unsigned char input = _getch();
		switch (toupper(input))
		{
		case 'W':
			move_user(x, y--, UP, chracter);
			break;
		case 'A':
			move_user(x--, y, LEFT, chracter);
			break;
		case 'S':
			move_user(x, y++, DOWN, chracter);
			break;
		case 'D':
			move_user(x++, y, RIGHT, chracter);
			break;
		}
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case 72 :
				{
					Bullet bullet(x, y, UP);
					fire(bullet);
					break;
				}

			case 80 :
				{
					Bullet bullet(x, y, DOWN);
					fire(bullet);
					break;
				}

			case 75 :
				{
					Bullet bullet(x, y, LEFT);
					fire(bullet);
					break;
				}

			case 77 :
				{
					Bullet bullet(x, y, RIGHT);
					fire(bullet);
					break;
				}
			}
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
