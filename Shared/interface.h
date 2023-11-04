#pragma once

// 경기장 정보
const SMALL_RECT FIELD = { 4, 2, 60, 37 };

const SHORT FIELD_WIDTH = FIELD.Right - FIELD.Left - 1;
const SHORT FIELD_HEIGHT = FIELD.Bottom - FIELD.Top - 1;

// 보유 가능한 최대 액티브 스킬 수
const int MAX_ACTIVE_SKILL = 5;