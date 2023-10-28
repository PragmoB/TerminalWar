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
	// ȭ�鿡 ĳ���� ù ǥ��
	pos.X *= 2; // ���ι��� �̵��� 2ĭ������ �����
	pos.X += field.Left - 1;
	pos.Y += field.Top;
	this->pos = pos;
	this->appear();
}

void Player::appear()
{
	/* ü�� ���� ǥ������ ���� ��ġ ��� */

	// �⺻������ ü�� ���� ǥ������ ĳ���� 1ĭ �� ��� �����Ͽ� ǥ��
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < field.Left + 1) // ü�� ���� ǥ������ ����� ���� ���� �������´ٸ�
		pos_temp.X = field.Left + 1; // ����� ���� ������ ����
	else if ((signed)pos_temp.X > field.Left + 2 * field_width - 7) // ����� ������ ���� �������´ٸ�
		pos_temp.X = field.Left + 2 * field_width - 7; // ����� ������ ������ ����

	/* ü�� ���� => 5�ڸ� ���ڿ��� ��ȯ */

	char HP_str[6] = "";
	sprintf_s(HP_str, "%05d", HP);

	/* ��� */

	if (me)
		graphic.draw(pos, chracter, GREEN);
	else
		graphic.draw(pos, chracter);

	graphic.draw(pos_temp, "\u2665", DARK_RED); // ������ ��Ʈ
	pos_temp.X += 2;
	graphic.draw(pos_temp, HP_str, SKY_BLUE);
}
void Player::disappear()
{
	/* ü�� ���� ǥ������ �ִ� ��ġ ��� */

	// �⺻������ ü�� ���� ǥ������ ĳ���� 1ĭ �� ��� �����Ͽ� ǥ��
	COORD pos_temp = pos;
	pos_temp.X -= 3; pos_temp.Y += 1;
	
	if ((signed)pos_temp.X < field.Left + 1) // ü�� ���� ǥ������ ����� ���� ���� �������´ٸ�
		pos_temp.X = field.Left + 1; // ����� ���� ������ ����
	else if ((signed)pos_temp.X > field.Left + 2 * field_width - 7) // ����� ������ ���� �������´ٸ�
		pos_temp.X = field.Left + 2 * field_width - 7; // ����� ������ ������ ����

	/* ����� */
	
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

	// ���ι��� �̵��� �ܼ�â�� Ŀ�� �簢���� ���� ���� ���� ������ 2:1���� ����Ͽ� ������
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