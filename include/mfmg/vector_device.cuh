/*************************************************************************
 * Copyright (c) 2018 by the mfmg authors                                *
 * All rights reserved.                                                  *
 *                                                                       *
 * This file is part of the mfmg libary. mfmg is distributed under a BSD *
 * 3-clause license. For the licensing terms see the LICENSE file in the *
 * top-level directory                                                   *
 *                                                                       *
 * SPDX-License-Identifier: BSD-3-Clause                                 *
 *************************************************************************/

#ifndef VECTOR_DEVICE_CUH
#define VECTOR_DEVICE_CUH

#ifdef MFMG_WITH_CUDA

#include <mfmg/exceptions.hpp>

#include <deal.II/base/partitioner.h>
#include <memory>

namespace mfmg
{
/**
 * This structure encapsulates the pointer that defines the vector on the
 * device and the partitioner.
 */
template <typename ScalarType>
struct VectorDevice
{
  VectorDevice(ScalarType *values_dev,
               std::shared_ptr<const dealii::Utilities::MPI::Partitioner> part)
      : val_dev(values_dev), partitioner(part)
  {
  }

  ScalarType *val_dev;
  std::shared_ptr<const dealii::Utilities::MPI::Partitioner> partitioner;
};
}

#endif

#endif