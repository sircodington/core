//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/Types.h>

namespace core {

class StopWatch
{
public:
    StopWatch() = default;

    void start();
    [[nodiscard]] u64 elapsed_ms() const;

private:
    bool m_started { false };
    u64 m_start { 0 };
};

}  // namespace core

using core::StopWatch;
