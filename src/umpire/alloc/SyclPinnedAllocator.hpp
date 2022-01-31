//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-22, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////
#ifndef UMPIRE_SyclPinnedAllocator_HPP
#define UMPIRE_SyclPinnedAllocator_HPP

#include <CL/sycl.hpp>

#include "umpire/util/Macros.hpp"

namespace umpire {
namespace alloc {

/*!
 * \brief Uses sycl's malloc_host and free to allocate and deallocate
 *        pinned memory on host.
 */
struct SyclPinnedAllocator {
  /*!
   * \Brief Allocate bytes of memory using SYCL malloc_host
   *
   * \param size Number of bytes to allocate.
   * \param queue_t SYCL queue for providing information on device and context
   * \return Pointer to start of the allocation on host.
   *
   * \throws umpire::util::runtime_error if memory cannot be allocated.
   */
  void* allocate(std::size_t size, const sycl::queue& queue_t)
  {
    void* ptr = sycl::malloc_host(size, queue_t);

    UMPIRE_LOG(Debug, "(bytes=" << size << ") returning " << ptr);

    if (ptr == nullptr) {
      UMPIRE_ERROR(runtime_error, "SYCL malloc_host( bytes = " << size << " ) failed with error!");
    } else {
      return ptr;
    }
  }

  /*!
   * \brief Deallocate memory using SYCL free.
   *
   * \param ptr Address to deallocate.
   * \param queue_t SYCL queue this pointer was asociated with
   *
   * \throws umpire::util::runtime_error if memory cannot be free'd.
   */
  void deallocate(void* ptr, const sycl::queue& queue_t)
  {
    UMPIRE_LOG(Debug, "(ptr=" << ptr << ")");

    sycl::free(ptr, queue_t);
  }

  bool isAccessible(Platform p)
  {
    if (p == Platform::sycl || p == Platform::host)
      return true;
    else
      return false; // p is undefined
  }
};

} // end of namespace alloc
} // end of namespace umpire

#endif // UMPIRE_SyclPinnedAllocator_HPP
