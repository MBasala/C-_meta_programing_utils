//
// Created by MBasa on 11/30/2024.
//

#ifndef PIMPL_PTR_HPP
#define PIMPL_PTR_HPP

#include <memory>
#include <type_traits>
#include <utility>
#include <functional>

template<typename pub_interface>
struct PimplPtr {
  using unique_pimpl_ptr = std::unique_ptr<pub_interface>;
  using shared_pimpl_ptr = std::shared_ptr<pub_interface>;
  using weak_pimpl_ptr = std::weak_ptr<pub_interface>;
  using ref_pimpl_ptr = std::reference_wrapper<pub_interface>;

  ~PimplPtr()                           = default;
  PimplPtr(PimplPtr const&)            = default;
  PimplPtr(PimplPtr&&)                 = default;
  PimplPtr& operator=(PimplPtr const&) = default;
  PimplPtr& operator=(PimplPtr&&)      = default;
};

#endif //PIMPL_PTR_HPP
