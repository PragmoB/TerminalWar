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
		for (; 0 < distance && field.Top < pos.Y - 1; distance--)
		{
			pos.Y--; // �Ѿ��� ���� �̵�
			graphic.draw(pos, shape_vertical, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps); // �߻� �ӵ� ����
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	case DOWN:
		for (; 0 < distance && pos.Y + 1 < field.Bottom; distance--)
		{
			pos.Y++;
			graphic.draw(pos, shape_vertical, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	case LEFT:
		for (; 0 < distance && field.Left < pos.X - 2; distance--)
		{
			pos.X -= 2;
			graphic.draw(pos, shape_horizontal, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	case RIGHT:
		for (; 0 < distance && pos.X + 2 < field.Left + 2 * field_width; distance--)
		{
			pos.X += 2;
			graphic.draw(pos, shape_horizontal, SKY_BLUE); // �׷��ȿ� �ݿ�

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // �ܻ� �����
		}
		break;
	}
}