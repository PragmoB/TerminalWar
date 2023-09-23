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
		for (; 0 < distance && 1 < y; distance--)
		{
			y--; // 총알이 위로 이동
			graphic.frame[field.top + y][field.left + 1 + x] = shape_vertical; // 프레임에 반영

			Sleep(1000 / bps); // 발사 속도 조절
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	case DOWN:
		for (; 0 < distance && y < field_height; distance--)
		{
			y++;
			graphic.frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	case LEFT:
		for (; 0 < distance && 1 < x; distance--)
		{
			x--;
			graphic.frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	case RIGHT:
		for (; 0 < distance && x < field_width; distance--)
		{
			x++;
			graphic.frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			graphic.frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	}
}