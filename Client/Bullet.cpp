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
		for (; 0 < distance && 1 < y; distance--)
		{
			y--; // �Ѿ��� ���� �̵�
			graphic.frame[field.top + y][field.left + 1 + x] = shape_vertical; // �����ӿ� �ݿ�

			Sleep(1000 / bps); // �߻� �ӵ� ����
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case DOWN:
		for (; 0 < distance && y < field_height; distance--)
		{
			y++;
			graphic.frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case LEFT:
		for (; 0 < distance && 1 < x; distance--)
		{
			x--;
			graphic.frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	case RIGHT:
		for (; 0 < distance && x < field_width; distance--)
		{
			x++;
			graphic.frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // �ܻ� �����
		}
		break;
	}
}