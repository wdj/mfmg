/*************************************************************************
 * Copyright (c) 2017-2019 by the mfmg authors                           *
 * All rights reserved.                                                  *
 *                                                                       *
 * This file is part of the mfmg libary. mfmg is distributed under a BSD *
 * 3-clause license. For the licensing terms see the LICENSE file in the *
 * top-level directory                                                   *
 *                                                                       *
 * SPDX-License-Identifier: BSD-3-Clause                                 *
 *************************************************************************/

#include <mfmg/dealii/amge_host.templates.hpp>
#include <mfmg/dealii/dealii_matrix_free_mesh_evaluator.hpp>
#include <mfmg/dealii/dealii_mesh_evaluator.hpp>

#include <deal.II/lac/la_parallel_vector.h>

template class mfmg::AMGe_host<
    2, mfmg::DealIIMeshEvaluator<2>,
    dealii::LinearAlgebra::distributed::Vector<double>>;
template class mfmg::AMGe_host<
    3, mfmg::DealIIMeshEvaluator<3>,
    dealii::LinearAlgebra::distributed::Vector<double>>;
template class mfmg::AMGe_host<
    2, mfmg::DealIIMatrixFreeMeshEvaluator<2>,
    dealii::LinearAlgebra::distributed::Vector<double>>;
template class mfmg::AMGe_host<
    3, mfmg::DealIIMatrixFreeMeshEvaluator<3>,
    dealii::LinearAlgebra::distributed::Vector<double>>;
