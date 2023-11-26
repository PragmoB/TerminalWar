#pragma once

// 경기장 정보
const SMALL_RECT FIELD = { 4, 7, 60, 34 };

const SHORT FIELD_WIDTH = FIELD.Right - FIELD.Left - 1;
const SHORT FIELD_HEIGHT = FIELD.Bottom - FIELD.Top - 1;

// 보유 가능한 최대 액티브 스킬 수
const int MAX_ACTIVE_SKILL = 5;
// 플레이어 최대 레벨
const int MAX_LEVEL = 50;
// 플레이어 레벨 별 레벨업 에너지 요구량
const int REQUIRED_ENERGY[] = { 40, 60, 80, 100, 120, 140, 160, 180, 200, 220,
										   240, 260, 280, 300, 320, 340, 360, 380, 400, 440,
										   460, 480, 500, 520, 540, 560, 580, 600, 620, 640,
										   660, 680, 700, 720, 740, 760, 780, 800, 820, 840,
										   880, 900, 920, 940, 960, 980, 1000, 1020, 1040, 1060, };
// 플레이어 레벨 별 사망 시 에너지 반환량
const int HUNTING_ENERGY[] = { 40, 60, 80, 100, 120, 140, 160, 180, 200, 220,
										   240, 260, 280, 300, 320, 340, 360, 380, 400, 440,
										   460, 480, 500, 520, 540, 560, 580, 600, 620, 640,
										   660, 680, 700, 720, 740, 760, 780, 800, 820, 840,
										   880, 900, 920, 940, 960, 980, 1000, 1020, 1040, 1060, };
// 스킬 업그레이드 포기 시 에너지 환수 비율(백분율)
const int ENERGY_REFUND_RATE = 30;

// 스킬 강화 선택지 개수
const int NUM_UPGRADE_SKILL_OPTIONS = 3;

enum DIRECTION
{
	UP = 1, DOWN, LEFT, RIGHT
};