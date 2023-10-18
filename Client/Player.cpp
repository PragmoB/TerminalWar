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

Player::Player(COORD pos, char chracter) : chracter(chracter)
{
	// 화면에 캐릭터 첫 표시
	pos.X *= 2; // 가로방향 이동은 2칸씩임을 고려함
	pos.X += field.Left - 1;
	pos.Y += field.Top;
	this->pos = pos;
	graphic.draw(pos, chracter, GRAY);
}

void Player::move(DIRECTION dir)
{
	graphic.draw(pos, ' ', GRAY);

	switch (dir)
	{
	case UP: pos.Y--; break;
	case DOWN: pos.Y++; break;

	// 가로방향 이동은 콘솔창의 커서 사각형의 가로 세로 길이 비율이 2:1임을 고려하여 움직임
	case LEFT: pos.X -= 2; break;
	case RIGHT: pos.X += 2; break;
	}
	graphic.draw(pos, chracter, GRAY);
}

void Player::shoot(DIRECTION dir)
{
	graphic.fire_queue.Enqueue(Bullet(pos, dir));
	sound.shoot();
	return;
}

void Player::hit()
{
	graphic.draw(pos, '*', RED);
	sound.hit();
}

Player::~Player()
{
	graphic.draw(pos, ' ', GRAY);
}