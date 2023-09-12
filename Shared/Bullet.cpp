#include "Bullet.h"
#include "interface.h"

/* �Ѿ� ��ġ�� �����ϰ�, frame�� �ݿ��� */

void Bullet::fire(RECT field, char frame[FRAME_Y][FRAME_X])
{

	switch (dir)
	{
	case UP:
		// ���� ��ų� �����Ÿ��� 0�� �� ������ ��� ���ư�
		for (; 0 < distance && 0 < y - 1; distance--)
		{
			y--; // �Ѿ��� ���� �̵�
			if (frame) // �����ӿ� �ݿ�
				frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps); // �߻� �ӵ� ����
			frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case DOWN:
		for (; 0 < distance && y + 1 < field.bottom - field.top - 1; distance--)
		{
			y++;
			if (frame)
				frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps);
			frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case LEFT:
		for (; 0 < distance && 0 < x - 1; distance--)
		{
			x--;
			if (frame)
				frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case RIGHT:
		for (; 0 < distance && x + 1 < field.right - field.left - 3; distance--)
		{
			x++;
			if (frame)
				frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	}
}