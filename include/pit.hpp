#pragma once
#include <int.hpp>

#define PIT_ACCESS_MODE_LOHI 0b00110000
#define PIT_RATE_GENERATOR 0b00000100
#define PIT_BASE_FREQ 1193182
#define PIT_FREQ 1000
#define PIT_DIVISOR ((u16)(PIT_BASE_FREQ / PIT_FREQ))
#define PIT_CH0_PORT 0x40
#define PIT_CH1_PORT 0x41
#define PIT_CH2_PORT 0x42
#define PIT_COM_PORT 0x43

void init_pit();
void pit_handler();