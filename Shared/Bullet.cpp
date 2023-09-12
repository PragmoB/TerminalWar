#include "Bullet.h"
#include "interface.h"

/* 총알 위치를 조정하고, frame에 반영함 */

void Bullet::fire(RECT field, char frame[FRAME_Y][FRAME_X])
{

	switch (dir)
	{
	case UP:
		// 벽에 닿거나 사정거리가 0이 될 때까지 계속 날아감
		for (; 0 < distance && 0 < y - 1; distance--)
		{
			y--; // 총알이 위로 이동
			if (frame) // 프레임에 반영
				frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps); // 발사 속도 조절
			frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	case DOWN:
		for (; 0 < distance && y + 1 < field.bottom - field.top - 1; distance--)
		{
			y++;
			if (frame)
				frame[field.top + y][field.left + 1 + x] = shape_vertical;

			Sleep(1000 / bps);
			frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	case LEFT:
		for (; 0 < distance && 0 < x - 1; distance--)
		{
			x--;
			if (frame)
				frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	case RIGHT:
		for (; 0 < distance && x + 1 < field.right - field.left - 3; distance--)
		{
			x++;
			if (frame)
				frame[field.top + y][field.left + 1 + x] = shape_horizontal;

			Sleep(1000 / bps);
			frame[field.top + y][field.left + 1 + x] = ' '; // 잔상 지우기
		}
		break;
	}
}