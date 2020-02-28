// ---------------------------------------------------------------------
//
// Copyright (c) 2011 - 2019 by the IBAMR developers
// All rights reserved.
//
// This file is part of IBAMR.
//
// IBAMR is free software and is distributed under the 3-clause BSD
// license. The full text of the license can be found in the file
// COPYRIGHT at the top level directory of IBAMR.
//
// ---------------------------------------------------------------------

#ifndef included_IBTK_PETScVecUtilities
#define included_IBTK_PETScVecUtilities

/////////////////////////////// INCLUDES /////////////////////////////////////

#include "tbox/Pointer.h"

#include "petscao.h"
#include "petscvec.h"

#include <RefineSchedule.h>

#include <vector>

namespace SAMRAI
{
namespace hier
{
template <int DIM>
class PatchLevel;
} // namespace hier
} // namespace SAMRAI

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class PETScVecUtilities provides utility functions for <A
 * HREF="http://www.mcs.anl.gov/petsc">PETSc</A> Vec objects.
 */
class PETScVecUtilities
{
public:
    /*!
     * \name Methods acting on SAMRAI::hier::PatchLevel and
     * SAMRAI::hier::Variable objects.
     */
    //\{

    /*!
     * \brief Copy data to a parallel PETSc Vec on the provided
     * SAMRAI::hier::PatchLevel.
     */
    static void copyToPatchLevelVec(Vec& vec,
                                    int data_idx,
                                    int dof_index_idx,
                                    SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Copy data from a parallel PETSc Vec on the provided
     * SAMRAI::hier::PatchLevel.
     */
    static void copyFromPatchLevelVec(Vec& vec,
                                      int data_idx,
                                      int dof_index_idx,
                                      SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level,
                                      SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> > data_synch_sched,
                                      SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> > ghost_fill_sched);

    /*!
     * \brief Construct a RefineSchedule to synchronize shared values that can
     * be used in conjunction with copyFromPatchLevelVec().
     *
     * \note Synchronization is not required for all data centerings.  For cases
     * in which it is not needed, this method will return a NULL pointer.
     */
    static SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> >
    constructDataSynchSchedule(int data_idx, SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Construct a RefineSchedule to fill ghost cell values that can be
     * used in conjunction with copyFromPatchLevelVec().
     */
    static SAMRAI::tbox::Pointer<SAMRAI::xfer::RefineSchedule<NDIM> >
    constructGhostFillSchedule(int data_idx, SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Assign a DOF index to each unique degree of freedom on a
     * SAMRAI::hier::PatchLevel.
     *
     * The DOF indices generated by this method are compatible with the parallel
     * PETSc Vec objects generated by constructPatchLevelVec().
     *
     * \note DOF indices are \em not assigned to ghost cell values by this
     * method.
     */
    static void constructPatchLevelDOFIndices(std::vector<int>& num_dofs_per_proc,
                                              int dof_index_idx,
                                              SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Create an application ordering object (AO) that creates a mapping
     * between global PETSc indices and mapping of SAMRAI data indices for patch
     * data index \em dof_index_idx to a nonnegative integer \f$ (i,j,k,d):-> p \f$
     * on a SAMRAI::hier::PatchLevel.
     *
     *\see IndexUtilities::mapIndexToInteger() for
     * details on the integer mapping of SAMRAI data indices.
     *
     * \param ao_offset An integer offset used for integer mapping of SAMRAI
     * data indices.
     */
    static void constructPatchLevelAO(AO& ao,
                                      std::vector<int>& num_dofs_per_proc,
                                      int dof_index_idx,
                                      SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level,
                                      const int ao_offset = 0);

    //\}

protected:
private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    PETScVecUtilities() = delete;

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    PETScVecUtilities(const PETScVecUtilities& from) = delete;

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    PETScVecUtilities& operator=(const PETScVecUtilities& that) = delete;

    /*!
     * \brief Implementation of copyToPatchLevelVec() for cell-centered data.
     */
    static void copyToPatchLevelVec_cell(Vec& vec,
                                         int data_idx,
                                         int dof_index_idx,
                                         SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Implementation of copyToPatchLevelVec() for side-centered data.
     */
    static void copyToPatchLevelVec_side(Vec& vec,
                                         int data_idx,
                                         int dof_index_idx,
                                         SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Implementation of copyFromPatchLevelVec() for cell-centered data.
     */
    static void copyFromPatchLevelVec_cell(Vec& vec,
                                           int data_idx,
                                           int dof_index_idx,
                                           SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Implementation of copyFromPatchLevelVec() for side-centered data.
     */
    static void copyFromPatchLevelVec_side(Vec& vec,
                                           int data_idx,
                                           int dof_index_idx,
                                           SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Implementation of constructPatchLevelDOFIndices() for
     * cell-centered data.
     */
    static void constructPatchLevelDOFIndices_cell(std::vector<int>& num_dofs_proc,
                                                   int dof_index_idx,
                                                   SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Implementation of constructPatchLevelDOFIndices() for
     * side-centered data.
     */
    static void constructPatchLevelDOFIndices_side(std::vector<int>& num_dofs_proc,
                                                   int dof_index_idx,
                                                   SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level);

    /*!
     * \brief Implementation of constructPatchLevelAO for cell-centered data.
     */
    static void constructPatchLevelAO_cell(AO& ao,
                                           std::vector<int>& num_dofs_per_proc,
                                           const int dof_index_idx,
                                           SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level,
                                           const int ao_offset);

    /*!
     * \brief Implementation of constructPatchLevelAO for side-centered data.
     */
    static void constructPatchLevelAO_side(AO& ao,
                                           std::vector<int>& num_dofs_per_proc,
                                           const int dof_index_idx,
                                           SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level,
                                           const int ao_offset);
};
} // namespace IBTK

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBTK_PETScVecUtilities
