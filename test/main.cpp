//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <cstdlib>

#include "test_lexical_file.h"
#include "test_string_view.h"

int main()
{
    core::test_string_view();
    core::test_lexical_file();
    return EXIT_SUCCESS;
}
