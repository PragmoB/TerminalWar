#pragma once

#define FRAME_X 150
#define FRAME_Y 38

const RECT field = { 10, 4, 60, 37 };

const int field_width = field.right - field.left - 2;
const int field_height = field.bottom - field.top - 1;