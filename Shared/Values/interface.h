#pragma once

#define _WINSOCKAPI_
#include <Windows.h>

// ����� ����
const SMALL_RECT FIELD = { 4, 7, 60, 34 };

const SHORT FIELD_WIDTH = FIELD.Right - FIELD.Left - 1;
const SHORT FIELD_HEIGHT = FIELD.Bottom - FIELD.Top - 1;