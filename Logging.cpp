//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "Logging.h"

#include <cstdio>

namespace core {

void Logger<const char *>::log(const char *string) { printf("%s", string); }

void Logger<s8>::log(s8 value) { printf("%" PRId8, value); }
void Logger<s16>::log(s16 value) { printf("%" PRId16, value); }
void Logger<s32>::log(s32 value) { printf("%" PRId32, value); }
void Logger<s64>::log(s64 value) { printf("%" PRId64, value); }

void Logger<u8>::log(u8 value) { printf("%" PRIu8, value); }
void Logger<u16>::log(u16 value) { printf("%" PRIu16, value); }
void Logger<u32>::log(u32 value) { printf("%" PRIu32, value); }
void Logger<u64>::log(u64 value) { printf("%" PRIu64, value); }

#define PRIf32 "f"
#define PRIf64 "f"
void Logger<float>::log(float value) { printf("%" PRIf32, value); }
void Logger<double>::log(double value) { printf("%" PRIf64, value); }

}  // namespace core
