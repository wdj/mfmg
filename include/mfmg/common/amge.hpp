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

#ifndef AMGE_HPP
#define AMGE_HPP

#include <deal.II/base/mpi.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/lac/la_parallel_vector.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <deal.II/lac/trilinos_sparsity_pattern.h>

#include <boost/property_tree/ptree.hpp>

#include <array>
#include <map>
#include <string>

namespace mfmg
{
template <int dim, typename VectorType>
class AMGe
{
public:
  AMGe(MPI_Comm comm, dealii::DoFHandler<dim> const &dof_handler);

  /**
   * Flag cells to create agglomerates. This function returns the local number
   * of agglomerates that have been created.
   */
  unsigned int
  build_agglomerates(boost::property_tree::ptree const &ptree) const;

  /**
   * Create a Triangulation \p agglomerate_triangulation associated with an
   * agglomerate of a given \p agglomerate_id and a map that matches cells in
   * the local triangulation with cells in the global triangulation.
   */
  void build_agglomerate_triangulation(
      unsigned int agglomerate_id,
      dealii::Triangulation<dim> &agglomerate_triangulation,
      std::map<typename dealii::Triangulation<dim>::active_cell_iterator,
               typename dealii::DoFHandler<dim>::active_cell_iterator>
          &agglomerate_to_global_tria_map) const;

  /**
   * Compute the map between the dof indices of the local DoFHandler and the
   * dof indices of the global DoFHandler.
   */
  std::vector<dealii::types::global_dof_index> compute_dof_index_map(
      std::map<typename dealii::Triangulation<dim>::active_cell_iterator,
               typename dealii::DoFHandler<dim>::active_cell_iterator> const
          &patch_to_global_map,
      dealii::DoFHandler<dim> const &agglomerate_dof_handler) const;

  /**
   * Output the mesh and the agglomerate ids.
   */
  void output(std::string const &filename) const;

  void compute_restriction_sparse_matrix(
      std::vector<dealii::Vector<typename VectorType::value_type>> const
          &eigenvectors,
      std::vector<std::vector<typename VectorType::value_type>> const
          &diag_elements,
      std::vector<std::vector<dealii::types::global_dof_index>> const
          &dof_indices_maps,
      std::vector<unsigned int> const &n_local_eigenvectors,
      dealii::LinearAlgebra::distributed::Vector<
          typename VectorType::value_type> const
          &locally_relevant_global_diag_dev,
      dealii::TrilinosWrappers::SparseMatrix &restriction_sparse_matrix) const;

protected:
  MPI_Comm _comm;
  dealii::DoFHandler<dim> const &_dof_handler;

private:
  /**
   * Flag cells to create agglomerates. The desired size of the agglomerates is
   * given by \p agglomerate_dim. This function returns the local number of
   * agglomerates that have been created.
   */
  unsigned int build_agglomerates_block(
      std::array<unsigned int, dim> const &agglomerate_dim) const;

  /**
   * Flag cells to create agglomerates. \p partitioner_type is the partitioner
   * used (zoltan or metis) and \p n_agglomerates is the local number of
   * agglomerates that we would like to have. This function returns the local
   * number of agglomerates that have been created.
   */
  unsigned int
  build_agglomerates_partitioner(std::string const &partitioner_type,
                                 unsigned int n_agglomerates) const;

  dealii::TrilinosWrappers::SparsityPattern
  compute_restriction_sparsity_pattern(
      std::vector<dealii::Vector<double>> const &eigenvectors,
      std::vector<std::vector<dealii::types::global_dof_index>> const
          &dof_indices_maps,
      std::vector<unsigned int> const &n_local_eigenvectors) const;
};
} // namespace mfmg

#endif
