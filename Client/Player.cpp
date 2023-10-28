#include <cstdio>
#include <windows.h>
#include <mmsystem.h>

#include "interface.h"
#include "Graphic.h"
#include "Sound.h"
#include "Player.h"
#include "Bullet.h"

extern Graphic graphic;
extern Sound sound;

Player::Player(COORD pos, int HP, char chracter, bool me) : chracter(chracter), HP(HP), me(me)
{
	// 화면에 캐릭터 첫 표시
	pos.X *= 2; // 가로방향 이동은 2칸씩임을 고려함
	pos.X += field.Left - 1;
	pos.Y += field.Top;
	this->pos = pos;
	this->appear();
}

void Player::appear()
{
	/* 체력 상태 표시줄이 나올 위치 계산 */

	// 기본적으로 체력 상태 표시줄은 캐릭터 1칸 밑 가운데 정렬하여 표시
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < field.Left + 1) // 체력 상태 표시줄이 경기장 왼쪽 끝을 삐져나온다면
		pos_temp.X = field.Left + 1; // 경기장 왼쪽 끝으로 맞춤
	else if ((signed)pos_temp.X > field.Left + 2 * field_width - 7) // 경기장 오른쪽 끝을 삐져나온다면
		pos_temp.X = field.Left + 2 * field_width - 7; // 경기장 오른쪽 끝으로 맞춤

	/* 체력 숫자 => 5자리 문자열로 변환 */

	char HP_str[6] = "";
	sprintf_s(HP_str, "%05d", HP);

	/* 출력 */

	if (me)
		graphic.draw(pos, chracter, GREEN);
	else
		graphic.draw(pos, chracter);

	graphic.draw(pos_temp, "\u2665", DARK_RED); // 빨간색 하트
	pos_temp.X += 2;
	graphic.draw(pos_temp, HP_str, SKY_BLUE);
}
void Player::disappear()
{
	/* 체력 상태 표시줄이 있는 위치 계산 */

	// 기본적으로 체력 상태 표시줄은 캐릭터 1칸 밑 가운데 정렬하여 표시
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < field.Left + 1) // 체력 상태 표시줄이 경기장 왼쪽 끝을 삐져나온다면
		pos_temp.X = field.Left + 1; // 경기장 왼쪽 끝으로 맞춤
	else if ((signed)pos_temp.X > field.Left + 2 * field_width - 7) // 경기장 오른쪽 끝을 삐져나온다면
		pos_temp.X = field.Left + 2 * field_width - 7; // 경기장 오른쪽 끝으로 맞춤

	/* 지우기 */
	
	graphic.draw(pos, ' ');
	graphic.draw(pos_temp, "       ");
}
void Player::move(DIRECTION dir)
{
	this->disappear();

	switch (dir)
	{
	case UP: pos.Y--; break;
	case DOWN: pos.Y++; break;

	// 가로방향 이동은 콘솔창의 커서 사각형의 가로 세로 길이 비율이 2:1임을 고려하여 움직임
	case LEFT: pos.X -= 2; break;
	case RIGHT: pos.X += 2; break;
	}
	this->appear();
}

void Player::shoot(DIRECTION dir)
{
	graphic.fire_queue.Enqueue(Bullet(pos, dir));
	sound.shoot();
	return;
}

void Player::hit()
{
	HP -= 10;
	graphic.draw(pos, '*', RED);
	sound.hit();
}

bool Player::is_me()
{
	return me;
}

Player::~Player()
{
	this->disappear();
}