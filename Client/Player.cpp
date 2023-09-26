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

Player::Player(POINT pos, char chracter) : pos(pos), chracter(chracter)
{
	// 화면에 캐릭터 첫 표시
	graphic.frame[field.top + pos.y][field.left + 1 + pos.x] = chracter;
}

void Player::move(DIRECTION dir)
{
	graphic.frame[field.top + pos.y][field.left + 1 + pos.x] = ' ';

	switch (dir)
	{
	case UP:
		graphic.frame[field.top - 1 + pos.y][field.left + 1 + pos.x] = chracter;
		pos.y--;
		break;
	case DOWN:
		graphic.frame[field.top + 1 + pos.y][field.left + 1 + pos.x] = chracter;
		pos.y++;
		break;
	case LEFT:
		graphic.frame[field.top + pos.y][field.left + pos.x] = chracter;
		pos.x--;
		break;
	case RIGHT:
		graphic.frame[field.top + pos.y][field.left + 2 + pos.x] = chracter;
		pos.x++;
		break;
	}
}

void Player::shoot(DIRECTION dir)
{
	graphic.fire_queue.Enqueue(Bullet(pos.x, pos.y, dir));
	sound.shoot();
	return;
}

void Player::hit()
{
	graphic.frame[field.top + pos.y][field.left + 1 + pos.x] = '*';
	sound.hit();
}

Player::~Player()
{
	graphic.frame[field.top + pos.y][field.left + 1 + pos.x] = ' ';
}