//
// Created by MBasa on 11/30/2024.
//

#ifndef PIMPL_UNIQUE_HPP
#define PIMPL_UNIQUE_HPP
#include <memory>

namespace pimp_ptr_dmm
{
    template<typename, typename =std::allocator<void>> struct unique_impl;
}

template<typename pimpl_interface, typename allocator>
struct pimp_ptr_dmm::unique_impl {
    public:
        // Constructors
        unique_impl() noexcept = default;
        explicit unique_impl(pimpl_interface* ptr) noexcept : impl_ptr_(ptr) {}

        // Deleted copy operations
        unique_impl(const unique_impl&) = delete;
        unique_impl& operator=(const unique_impl&) = delete;

        // Defaulted move operations
        unique_impl(unique_impl&&) noexcept = default;
        unique_impl& operator=(unique_impl&&) noexcept = default;

        // Accessors
        pimpl_interface* get() const noexcept { return impl_ptr_.get(); }
        pimpl_interface* operator->() const noexcept { return impl_ptr_.get(); }
        pimpl_interface& operator*() const noexcept { return *impl_ptr_; }

        // Modifiers
        void reset(pimpl_interface* ptr = nullptr) noexcept { impl_ptr_.reset(ptr); }

        // Use count (always 1 for unique ownership)
        long use_count() const noexcept { return 1; }

    private:
        std::unique_ptr<pimpl_interface> impl_ptr_;
};

#endif //PIMPL_UNIQUE_HPP
