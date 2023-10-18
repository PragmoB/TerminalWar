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
	// ȭ�鿡 ĳ���� ù ǥ��
	pos.X *= 2; // ���ι��� �̵��� 2ĭ������ �����
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

	// ���ι��� �̵��� �ܼ�â�� Ŀ�� �簢���� ���� ���� ���� ������ 2:1���� ����Ͽ� ������
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