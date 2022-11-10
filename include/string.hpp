#pragma once
#include <va_list.hpp>

int vsprintf(char* dst, const char* fmt, va_list arg);
void sprintf(char* dst, const char* fmt, ...);