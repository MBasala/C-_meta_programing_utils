//
// Created by MBasa on 11/30/2024.
//

#ifndef PIMPL_SHARED_HPP
#define PIMPL_SHARED_HPP
#include <memory>

template<typename Impl>
class shared_impl{
public:
    // Constructors
    shared_impl() noexcept = default;
    explicit shared_impl(Impl* ptr) : impl_ptr_(ptr) {}

    // Defaulted copy and move operations
    shared_impl(const shared_impl&) noexcept = default;
    shared_impl& operator=(const shared_impl&) noexcept = default;
    shared_impl(shared_impl&&) noexcept = default;
    shared_impl& operator=(shared_impl&&) noexcept = default;

    // Accessors
    Impl* get() const noexcept { return impl_ptr_.get(); }
    Impl* operator->() const noexcept { return impl_ptr_.get(); }
    Impl& operator*() const noexcept { return *impl_ptr_; }

    // Modifiers
    void reset(Impl* ptr = nullptr) noexcept { impl_ptr_.reset(ptr); }

    // Use count
    long use_count() const noexcept { return impl_ptr_.use_count(); }

private:
    std::shared_ptr<Impl> impl_ptr_;
};

#endif //PIMPL_SHARED_HPP
