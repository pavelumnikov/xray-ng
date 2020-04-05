#include "catch/catch.hpp"
#include "corlib/math/details/sse/sse_vector.h"

using namespace xr;

TEST_CASE("sse_vector tests")
{
    SECTION("construct full vector4 from __m128")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        __m128 vec = _mm_set_ps(w, z, y, x);
        math::details::sse_vector v { vec };

        REQUIRE(v.get_x() == x);
        REQUIRE(v.get_y() == y);
        REQUIRE(v.get_z() == z);
        REQUIRE(v.get_w() == w);
    }

    SECTION("construct full vector4 from float array")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) float f[] = { x, y, z, w };
        math::details::sse_vector v { f };

        REQUIRE(v.get_x() == x);
        REQUIRE(v.get_y() == y);
        REQUIRE(v.get_z() == z);
        REQUIRE(v.get_w() == w);
    }

    SECTION("construct full vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v { x, y, z, w };
        REQUIRE(v.get_x() == x);
        REQUIRE(v.get_y() == y);
        REQUIRE(v.get_z() == z);
        REQUIRE(v.get_w() == w);
    }

    SECTION("construct full vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::float_in_vec fx { x };
        math::details::float_in_vec fy { y };
        math::details::float_in_vec fz { z };
        math::details::float_in_vec fw { w };

        math::details::sse_vector v { fx, fy, fz, fw };

        REQUIRE(v.get_x() == x);
        REQUIRE(v.get_y() == y);
        REQUIRE(v.get_z() == z);
        REQUIRE(v.get_w() == w);
    }

    SECTION("construct vector3")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v { x, y, z };
        REQUIRE(v.get_x() == x);
        REQUIRE(v.get_y() == y);
        REQUIRE(v.get_z() == z);
    }

    SECTION("copy vector")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 { v1 };

        REQUIRE(v2.get_x() == x);
        REQUIRE(v2.get_y() == y);
        REQUIRE(v2.get_z() == z);
        REQUIRE(v2.get_w() == w);
    }

    SECTION("assign vector")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 = v1;

        REQUIRE(v2.get_x() == x);
        REQUIRE(v2.get_y() == y);
        REQUIRE(v2.get_z() == z);
        REQUIRE(v2.get_w() == w);
    }

    SECTION("assign vector from __m128")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        __m128 vec = _mm_set_ps(w, z, y, x);
        math::details::sse_vector v2 = vec;

        REQUIRE(v2.get_x() == x);
        REQUIRE(v2.get_y() == y);
        REQUIRE(v2.get_z() == z);
        REQUIRE(v2.get_w() == w);
    }

    SECTION("negate vector")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 = -v1;

        REQUIRE(v2.get_x() == -x);
        REQUIRE(v2.get_y() == -y);
        REQUIRE(v2.get_z() == -z);
        REQUIRE(v2.get_w() == -w);
    }

    SECTION("equal_all vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 { x, y, z, w };

        REQUIRE(math::details::sse_vector::equal_all(v1, v2) == true);
    }

    SECTION("equal_all (not equals) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 { w, z, y, x };

        REQUIRE(math::details::sse_vector::equal_all(v1, v2) == false);
    }

    SECTION("equal_any vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 { x, w, y, w };

        REQUIRE(math::details::sse_vector::equal_any(v1, v2) == true);
    }

    SECTION("equal_any (not equals) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;
        constexpr float w = 4.0f;

        math::details::sse_vector v1 { x, y, z, w };
        math::details::sse_vector v2 { w, z, y, x };

        REQUIRE(math::details::sse_vector::equal_any(v1, v2) == false);
    }

    SECTION("equal_all_v3 vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { x, y, z };
        math::details::sse_vector v2 { x, y, z };

        REQUIRE(math::details::sse_vector::equal_all_v3(v1, v2) == true);
    }

    SECTION("equal_all_v3 (not equals) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { x, y, z };
        math::details::sse_vector v2 { z, y, x };

        REQUIRE(math::details::sse_vector::equal_all_v3(v1, v2) == false);
    }

    SECTION("equal_any_v3 (one value) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { x, y, z };
        math::details::sse_vector v2 { x, z, y };

        REQUIRE(math::details::sse_vector::equal_any_v3(v1, v2) == true);
    }

    SECTION("equal_any_v3 (two values: 1, 2) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { x, y, z };
        math::details::sse_vector v2 { x, y, y };

        REQUIRE(math::details::sse_vector::equal_any_v3(v1, v2) == true);
    }

    SECTION("equal_any_v3 (two values: 1, 3) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { x, z, z };
        math::details::sse_vector v2 { x, x, z };

        REQUIRE(math::details::sse_vector::equal_any_v3(v1, v2) == true);
    }

    SECTION("equal_any_v3 (two values: 2, 3) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { y, y, z };
        math::details::sse_vector v2 { z, y, z };

        REQUIRE(math::details::sse_vector::equal_any_v3(v1, v2) == true);
    }

    SECTION("equal_any_v3 (not equals) vector4")
    {
        constexpr float x = 1.0f;
        constexpr float y = 2.0f;
        constexpr float z = 3.0f;

        math::details::sse_vector v1 { x, y, z };
        math::details::sse_vector v2 { y, z, x };

        REQUIRE(math::details::sse_vector::equal_any_v3(v1, v2) == false);
    }
}