#pragma once

using i8 = char;
using u8 = unsigned char;
using i16 = short;
using u16 = unsigned short;
using i32 = int;
using u32 = unsigned int;
using size_t = __SIZE_TYPE__;
static_assert(sizeof(size_t) == sizeof(u32), "inconsistent type size");