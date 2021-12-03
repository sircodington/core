//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "Formatter.h"

#include <core/StringBuilder.h>

namespace core {

void Formatter<char>::format(StringBuilder &b, char value) { b.append(value); }
void Formatter<char>::format(StringBuilder &b, const char *value)
{
    b.append(value);
}

void Formatter<s8>::format(StringBuilder &b, s8 value) { b.append(value); }
void Formatter<s16>::format(StringBuilder &b, s16 value) { b.append(value); }
void Formatter<s32>::format(StringBuilder &b, s32 value) { b.append(value); }
void Formatter<s64>::format(StringBuilder &b, s64 value) { b.append(value); }

void Formatter<u8>::format(StringBuilder &b, u8 value) { b.append(value); }
void Formatter<u16>::format(StringBuilder &b, u16 value) { b.append(value); }
void Formatter<u32>::format(StringBuilder &b, u32 value) { b.append(value); }
void Formatter<u64>::format(StringBuilder &b, u64 value) { b.append(value); }

void Formatter<float>::format(StringBuilder &b, float value)
{
    b.append(value);
}
void Formatter<double>::format(StringBuilder &b, double value)
{
    b.append(value);
}

}  // namespace core