// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

namespace xr::math
{

template<typename Dimension>
struct rect final
{
    using type = Dimension;
    type x {}, y {};
    type width {}, height {};

    XR_DECLARE_DEFAULT_CONSTEXPR_CLASS(rect);
}; // struct rect

using int_rect = rect<int32_t>;
using uint_rect = rect<uint32_t>;
using float_rect = rect<float>;

} // namespace xr::math
