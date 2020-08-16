// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/hash_elsa.h"
#include "EASTL/internal/copy_help.h"

//-----------------------------------------------------------------------------------------------------------
namespace eastl
{
    template<typename T> struct hash;
} // namespace eastl
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename Type>
class pre_hashed final
{
public:
    using type = Type;
    using reference = eastl::add_lvalue_reference_t<Type>;
    using const_reference = eastl::add_lvalue_reference_t<eastl::add_const_t<Type>>;

    explicit constexpr pre_hashed(type const& data);

    friend constexpr bool operator == (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs);
    friend constexpr bool operator != (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs);

    friend constexpr bool operator == (const_reference lhs, pre_hashed<Type> const& rhs);
    friend constexpr bool operator != (const_reference lhs, pre_hashed<Type> const& rhs);

    friend constexpr bool operator == (pre_hashed<Type> const& lhs, const_reference rhs);
    friend constexpr bool operator != (pre_hashed<Type> const& lhs, const_reference rhs);

    constexpr reference data() const;
    constexpr size_t hash() const;

private:
    type m_data;
    size_t m_hash;
}; // class pre_hashed<Type>

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr pre_hashed<Type>::pre_hashed(type const& data)
    : m_data { data }
    , m_hash { hashing::elsa<Type>{}(m_data) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator == (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs)
{
    return lhs.m_hash == rhs.m_hash;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator != (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs)
{
    return lhs.m_hash == rhs.m_hash;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator == (typename pre_hashed<Type>::const_reference lhs,
    pre_hashed<Type> const& rhs)
{
    auto hash = hashing::elsa<Type>()(lhs);
    return hash == rhs.m_hash;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator != (typename pre_hashed<Type>::const_reference lhs,
    pre_hashed<Type> const& rhs)
{
    auto hash = hashing::elsa<Type>()(lhs);
    return hash != rhs.m_hash;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator == (pre_hashed<Type> const& lhs,
    typename pre_hashed<Type>::const_reference rhs)
{
    auto hash = hashing::elsa<Type>()(rhs);
    return hash == lhs.m_hash;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator != (pre_hashed<Type> const& lhs,
    typename pre_hashed<Type>::const_reference rhs)
{
    auto hash = hashing::elsa<Type>()(rhs);
    return hash == lhs.m_hash;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr pre_hashed<Type>::reference
pre_hashed<Type>::data() const
{
    return m_data;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr size_t pre_hashed<Type>::hash() const
{
    return m_hash;
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
