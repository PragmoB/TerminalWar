#include "Bullet.h"
#include "Graphic.h"
#include "interface.h"

extern Graphic graphic;

/* �Ѿ� ��ġ�� �����ϰ�, graphic�� �ݿ��� */

void Bullet::fire()
{

	switch (dir)
	{
	case UP:
		// ���� ��ų� �����Ÿ��� 0�� �� ������ ��� ���ư�
		for (; 0 < distance && 0 < y - 1; distance--)
		{
			y--; // �Ѿ��� ���� �̵�
			if (graphic.frame) // �����ӿ� �ݿ�
				graphic.frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps); // �߻� �ӵ� ����
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case DOWN:
		for (; 0 < distance && y + 1 < field.bottom - field.top - 1; distance--)
		{
			y++;
			if (graphic.frame)
				graphic.frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case LEFT:
		for (; 0 < distance && 0 < x - 1; distance--)
		{
			x--;
			if (graphic.frame)
				graphic.frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case RIGHT:
		for (; 0 < distance && x + 1 < field.right - field.left - 3; distance--)
		{
			x++;
			if (graphic.frame)
				graphic.frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	}
}