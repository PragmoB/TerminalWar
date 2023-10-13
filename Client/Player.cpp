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
	pos.X += field.Left;
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
	case LEFT: pos.X--; break;
	case RIGHT: pos.X++; break;
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