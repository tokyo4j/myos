#pragma once

#define PORT 0x3f8 // COM1

int init_uart();
int is_transmit_empty();
void write_serial(char a);