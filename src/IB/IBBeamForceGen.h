#ifndef included_IBBeamForceGen
#define included_IBBeamForceGen

// Filename: IBBeamForceGen.h
// Last modified: <30.Dec.2009 19:50:50 griffith@boyce-griffiths-mac-pro.local>
// Created on 22 Mar 2007 by Boyce Griffith (griffith@box221.cims.nyu.edu)

/////////////////////////////// INCLUDES /////////////////////////////////////

// IBAMR INCLUDES
#include <ibamr/IBLagrangianForceStrategy.h>

// PETSc INCLUDES
#include <petscmat.h>

// C++ STDLIB INCLUDES
#include <vector>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class IBBeamForceGen is a concrete IBLagrangianForceStrategy that
 * computes the forces generated by a collection of linear beams (i.e.,
 * structures that resist bending).
 *
 * \note Class IBBeamForceGen DOES NOT correct for periodic displacements of IB
 * points; however, when used in conjuction with class IBStandardForceGen,
 * periodic displacements are handled correctly.
 *
 * \see IBBeamForceSpec
 */
class IBBeamForceGen
    : public IBLagrangianForceStrategy
{
public:
    /*!
     * \brief Default constructor.
     */
    IBBeamForceGen(
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db=NULL);

    /*!
     * \brief Virtual destructor.
     */
    virtual
    ~IBBeamForceGen();

    /*!
     * \brief Setup the data needed to compute the beam forces on the specified
     * level of the patch hierarchy.
     */
    virtual void
    initializeLevelData(
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double init_data_time,
        const bool initial_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the beam forces generated by the Lagrangian structure on
     * the specified level of the patch hierarchy.
     *
     * \note Nodal forces computed by this method are \em added to the force
     * vector.
     */
    virtual void
    computeLagrangianForce(
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> F_data,
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> X_data,
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> U_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the non-zero structure of the force Jacobian matrix.
     *
     * \note Elements indices must be global PETSc indices.
     */
    virtual void
    computeLagrangianForceJacobianNonzeroStructure(
        std::vector<int>& d_nnz,
        std::vector<int>& o_nnz,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the Jacobian of the force with respect to the present
     * structure configuration and velocity.
     *
     * \note The elements of the Jacobian should be "accumulated" in the
     * provided matrix J.
     *
     * \note A default implementation is provided with results in an assertion
     * failure for structures for which no Jacobian is available.
     */
    virtual void
    computeLagrangianForceJacobian(
        Mat& J_mat,
        MatAssemblyType assembly_type,
        const double X_coef,
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> X_data,
        const double U_coef,
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> U_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

    /*!
     * \brief Compute the potential energy with respect to the present structure
     * configuration and velocity.
     *
     * \note This method is not actually implemented --- it just prints a
     * warning message and returns 0.0.
     */
    virtual double
    computeLagrangianEnergy(
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> X_data,
        SAMRAI::tbox::Pointer<IBTK::LNodeLevelData> U_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
        const int level_number,
        const double data_time,
        IBTK::LDataManager* const lag_manager);

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    IBBeamForceGen(
        const IBBeamForceGen& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    IBBeamForceGen&
    operator=(
        const IBBeamForceGen& that);

    /*!
     * \brief Read input values, indicated above, from given database.
     *
     * The database pointer may be null.
     */
    void
    getFromInput(
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> db);

    /*!
     * \name Data maintained separately for each level of the patch hierarchy.
     */
    //\{
    std::vector<Mat> d_D_next_mats, d_D_prev_mats;
    std::vector<std::vector<int> > d_petsc_mastr_node_idxs, d_petsc_next_node_idxs, d_petsc_prev_node_idxs;
    std::vector<std::vector<double> > d_bend_rigidities;
    std::vector<std::vector<std::vector<double> > > d_mesh_dependent_curvatures;
    std::vector<bool> d_is_initialized;
    //\}
};
}// namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibamr/IBBeamForceGen.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBBeamForceGen
