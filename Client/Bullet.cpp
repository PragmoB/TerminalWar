#include "Bullet.h"
#include "Graphic.h"
#include "interface.h"

extern Graphic graphic;

/* 총알 위치를 조정하고, graphic에 반영함 */

void Bullet::fire()
{
	switch (dir)
	{
	case UP:
		// 벽에 닿거나 사정거리가 0이 될 때까지 계속 날아감
		for (; 0 < distance && field.Top < pos.Y - 1; distance--)
		{
			pos.Y--; // 총알이 위로 이동
			graphic.draw(pos, shape_vertical, SKY_BLUE); // 그래픽에 반영

			Sleep(1000 / bps); // 발사 속도 조절
			graphic.draw(pos, ' ', SKY_BLUE); // 잔상 지우기
		}
		break;
	case DOWN:
		for (; 0 < distance && pos.Y + 1 < field.Bottom; distance--)
		{
			pos.Y++;
			graphic.draw(pos, shape_vertical, SKY_BLUE); // 그래픽에 반영

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // 잔상 지우기
		}
		break;
	case LEFT:
		for (; 0 < distance && field.Left < pos.X - 2; distance--)
		{
			pos.X -= 2;
			graphic.draw(pos, shape_horizontal, SKY_BLUE); // 그래픽에 반영

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // 잔상 지우기
		}
		break;
	case RIGHT:
		for (; 0 < distance && pos.X + 2 < field.Left + 2 * field_width; distance--)
		{
			pos.X += 2;
			graphic.draw(pos, shape_horizontal, SKY_BLUE); // 그래픽에 반영

			Sleep(1000 / bps);
			graphic.draw(pos, ' ', SKY_BLUE); // 잔상 지우기
		}
		break;
	}
}