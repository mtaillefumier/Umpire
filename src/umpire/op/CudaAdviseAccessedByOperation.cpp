//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-22, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////
#include "umpire/op/CudaAdviseAccessedByOperation.hpp"

#include <cuda_runtime_api.h>

#include "umpire/util/Macros.hpp"

namespace umpire {
namespace op {

void CudaAdviseAccessedByOperation::apply(void* src_ptr, util::AllocationRecord* UMPIRE_UNUSED_ARG(src_allocation),
                                          int val, std::size_t length)
{
  int device = val;
  cudaError_t error;

  cudaDeviceProp properties;
  error = ::cudaGetDeviceProperties(&properties, 0);

  if (error != cudaSuccess) {
    UMPIRE_ERROR(runtime_error,
                 "cudaGetDeviceProperties( device = " << 0 << "),"
                                                      << " failed with error: " << cudaGetErrorString(error));
  }

  if (properties.managedMemory == 1 && properties.concurrentManagedAccess == 1) {
    error = ::cudaMemAdvise(src_ptr, length, cudaMemAdviseSetAccessedBy, device);

    if (error != cudaSuccess) {
      UMPIRE_ERROR(runtime_error, "cudaMemAdvise( src_ptr = " << src_ptr << ", length = " << length
                                                              << ", cudaMemAdviseSetAccessedBy, " << device
                                                              << ") failed with error: " << cudaGetErrorString(error));
    }
  }
}

} // end of namespace op
} // end of namespace umpire
