#include <windows.h>

#include "interface.h"
#include "Graphic.h"
#include "Player.h"
#include "Bullet.h"

extern Graphic graphic;

Player::Player(POINT pos, char chracter) : pos(pos), chracter(chracter)
{
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
	return;
}

void Player::hit()
{
	graphic.frame[field.top + pos.y][field.left + 1 + pos.x] = '*';
}

Player::~Player()
{
	graphic.frame[field.top + pos.y][field.left + 1 + pos.x] = ' ';
}