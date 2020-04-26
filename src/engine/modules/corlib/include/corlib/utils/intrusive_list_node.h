// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename T>
class intrusive_list_node
{
public:
    typedef T* pointer;
    typedef T& reference;

    intrusive_list_node() XR_NOEXCEPT;
    ~intrusive_list_node();

    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(intrusive_list_node);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(intrusive_list_node);

    void append_before_this(reference node) XR_NOEXCEPT;
    void append_after_this(reference node) XR_NOEXCEPT;
    void remove_this() XR_NOEXCEPT;

    bool is_head() const XR_NOEXCEPT;
    bool has_next() const XR_NOEXCEPT;
    bool has_prev() const XR_NOEXCEPT;

    reference next() const XR_NOEXCEPT;
    reference prev() const XR_NOEXCEPT;

private:
    pointer m_next;
    pointer m_prev;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
intrusive_list_node<T>::intrusive_list_node() XR_NOEXCEPT
    : m_next(nullptr)
    , m_prev(nullptr)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
intrusive_list_node<T>::~intrusive_list_node()
{
    remove_this();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
void intrusive_list_node<T>::append_before_this(reference node) XR_NOEXCEPT
{
    if(has_prev()) // change previous node if exists
    {
        auto& p = prev();
        p.m_next = &node;
        node.m_prev = &p;
    }

    m_prev = &node;
    node.m_next = static_cast<pointer>(this);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
void intrusive_list_node<T>::append_after_this(reference node) XR_NOEXCEPT
{
    if(has_next()) // change next node if exists
    {
        auto& p = next();
        p.m_prev = &node;
        node.m_next = &p;
    }

    m_next = &node;
    node.m_prev = static_cast<pointer>(this);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
void intrusive_list_node<T>::remove_this() XR_NOEXCEPT
{
    if(m_prev)
        m_prev->m_next = m_next;

    if(m_next)
        m_next->m_prev = m_prev;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
bool intrusive_list_node<T>::is_head() const XR_NOEXCEPT
{
    return m_prev == nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T>
bool intrusive_list_node<T>::has_next() const XR_NOEXCEPT
{
    return m_next != nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T>
bool intrusive_list_node<T>::has_prev() const XR_NOEXCEPT
{
    return m_prev != nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
typename intrusive_list_node<T>::reference
intrusive_list_node<T>::next() const XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION(has_next());
    return *m_next;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T>
typename intrusive_list_node<T>::reference
intrusive_list_node<T>::prev() const XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION(has_prev());
    return *m_prev;
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
