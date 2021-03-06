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

#ifndef MFMG_DEALII_MATRIX_FREE_MESH_EVALUATOR_HPP
#define MFMG_DEALII_MATRIX_FREE_MESH_EVALUATOR_HPP

#include <mfmg/dealii/dealii_mesh_evaluator.hpp>

#include <deal.II/dofs/dof_handler.h>
#include <deal.II/lac/affine_constraints.h>
#include <deal.II/lac/la_parallel_vector.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <deal.II/lac/vector.h>

#include <type_traits>

namespace mfmg
{
template <int dim>
class DealIIMatrixFreeMeshEvaluator : public DealIIMeshEvaluator<dim>
{
public:
  using size_type = dealii::types::global_dof_index;
  static int constexpr _dim = dim;

  DealIIMatrixFreeMeshEvaluator(dealii::DoFHandler<dim> &dof_handler,
                                dealii::AffineConstraints<double> &constraints);

  std::string get_mesh_evaluator_type() const override final;

  std::shared_ptr<dealii::LinearAlgebra::distributed::Vector<double>>
  build_range_vector() const;

  dealii::types::global_dof_index m() const;

  // FIXME throw an error to force the user to implement that member function in
  // the derived class
  virtual void
  matrix_free_evaluate_agglomerate(dealii::DoFHandler<dim> &dof_handler,
                                   dealii::Vector<double> const &src,
                                   dealii::Vector<double> &dst) const
  {
    dealii::SparsityPattern sparsity_pattern;
    dealii::SparseMatrix<double> system_matrix;
    dealii::AffineConstraints<double> constraints;
    this->evaluate_agglomerate(dof_handler, constraints, sparsity_pattern,
                               system_matrix);
    system_matrix.vmult(dst, src);
  }

  // FIXME throw an error to force the user to implement that member function in
  // the derived class
  virtual std::vector<double> matrix_free_get_agglomerate_diagonal(
      dealii::DoFHandler<dim> &dof_handler) const
  {
    dealii::AffineConstraints<double> constraints;
    dealii::SparsityPattern sparsity_pattern;
    dealii::SparseMatrix<double> system_matrix;
    this->evaluate_agglomerate(dof_handler, constraints, sparsity_pattern,
                               system_matrix);
    size_type const size = dof_handler.n_dofs();
    std::vector<double> diag_elements(size);
    for (size_type i = 0; i < size; ++i)
    {
      diag_elements[i] = system_matrix.diag_element(i);
    }
    return diag_elements;
  }

  virtual void matrix_free_evaluate_global(
      dealii::LinearAlgebra::distributed::Vector<double> const &src,
      dealii::LinearAlgebra::distributed::Vector<double> &dst) const
  {
    dealii::DoFHandler<dim> dof_handler;
    dealii::AffineConstraints<double> constraints;
    dealii::TrilinosWrappers::SparseMatrix system_matrix;
    this->evaluate_global(dof_handler, constraints, system_matrix);
    system_matrix.vmult(dst, src);
  }

  virtual dealii::LinearAlgebra::distributed::Vector<double>
  matrix_free_get_diagonal_inverse() const
  {
    dealii::DoFHandler<dim> dof_handler;
    dealii::AffineConstraints<double> constraints;
    dealii::TrilinosWrappers::SparseMatrix system_matrix;
    this->evaluate_global(dof_handler, constraints, system_matrix);
    dealii::IndexSet locally_owned_dofs =
        system_matrix.locally_owned_domain_indices();
    dealii::LinearAlgebra::distributed::Vector<double> diagonal(
        locally_owned_dofs, system_matrix.get_mpi_communicator());
    for (auto const index : locally_owned_dofs)
    {
      diagonal[index] = 1. / system_matrix.diag_element(index);
    }
    diagonal.compress(dealii::VectorOperation::insert);
    return diagonal;
  }
};

template <int dim>
struct is_matrix_free<DealIIMatrixFreeMeshEvaluator<dim>> : std::true_type
{
};

} // namespace mfmg

#endif
