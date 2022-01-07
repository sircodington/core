//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/StopWatch.h>

#include <cassert>
#include <chrono>

namespace core {

inline static u64 current_time()
{
    using namespace std::chrono;
    const auto time_point = high_resolution_clock::now();
    const auto duration_since_epoch = time_point.time_since_epoch();
    return duration_cast<milliseconds>(duration_since_epoch).count();
}

void core::StopWatch::start()
{
    assert(not m_started and "StopWatch already started");
    m_started = true;
    m_start = current_time();
}

u64 core::StopWatch::elapsed_ms() const
{
    assert(m_started and "StopWatch not started yet");
    return current_time() - m_start;
}

}  // namespace core