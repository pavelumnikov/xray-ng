#include "corlib/memory/memory_allocator_base.h"

namespace xr::memory
{

template<typename T>
class auto_free_ptr
{
public:
    auto_free_ptr() = default;
    ~auto_free_ptr();
    void reset(memory::base_allocator* alloc, T* ptr);

private:
    memory::base_allocator* m_allocator { nullptr };
    T* m_ptr { nullptr };
};

template<typename T>
auto_free_ptr<T>::~auto_free_ptr()
{
    if(m_ptr)
    {
        XR_DEALLOCATE_MEMORY_T(*m_allocator, T, "auto_free_ptr<T>");
        m_ptr = nullptr;
    }
}

template<typename T>
inline void
auto_free_ptr<T>::reset(memory::base_allocator* alloc, T* ptr)
{
    m_allocator = alloc;
    m_ptr = ptr;
}

} // namespace xr::memory