//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "Logging.h"

#include <cstdio>

namespace core {

void Logger<const char *>::log(const char *string) { printf("%s", string); }

void Logger<int>::log(int value) { printf("%d", value); }

}  // namespace core
