#pragma once

#include "protocol.h"

void print_frame();
void clear_frame();
void draw_field();
void welcome_user(uint32_t x, uint32_t y, char chracter);
void move_user(uint32_t x, uint32_t y, DIRECTION dir, char chracter);