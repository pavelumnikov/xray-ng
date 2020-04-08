// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

struct backoff_strategy_nothing final
{
    void operator()() const noexcept;
};

#if defined(__IME_PLATFORM_WIN64__)
inline void backoff_strategy_nothing::operator()() const noexcept
{
    (void)0;
}
#endif // defined(__IME_PLATFORM_WIN64__)


struct backoff_strategy_yield final
{
    void operator()() const noexcept;
};

struct backoff_strategy_pause final
{
    void operator()() const noexcept;
};

struct backoff_strategy_noop final
{
    void operator()() const noexcept;
};

#if defined(__IME_PLATFORM_WIN64__)
inline void backoff_strategy_noop::operator()() const noexcept
{
    (void)0;
}
#endif // defined(__IME_PLATFORM_WIN64__)

} // namespace xr::threading
//------------------------------------------------------------------------------