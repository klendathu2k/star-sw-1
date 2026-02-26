/*!
 * \class StHelixModel 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StHelixModel.h,v 2.10 2009/11/23 16:34:06 fisyak Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHelixModel.h,v $
 * Revision 2.10  2009/11/23 16:34:06  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.9  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.8  2003/04/09 17:59:39  genevb
 * Add setMomentum function
 *
 * Revision 2.7  2002/11/26 02:19:11  perev
 * StEventMaker ITTF modif
 *
 * Revision 2.6  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/07/17 22:23:30  ullrich
 * Added helicity to track geometry.
 *
 * Revision 2.4  2001/04/05 04:00:37  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.3  2001/03/24 03:34:49  perev
 * clone() -> clone() const
 *
 * Revision 2.2  1999/10/28 22:25:45  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:20  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHelixModel_hh
#define StHelixModel_hh
/// @file StHelixModel.h
/// @brief Concrete helix-based implementation of StTrackGeometry for a charged track in a uniform magnetic field.

#include "StTrackGeometry.h"
#include "StThreeVectorF.hh"

/// @brief Concrete helix-based implementation of StTrackGeometry for a charged track.
///
/// StHelixModel stores the five helix parameters (charge, curvature, ψ, dip angle,
/// origin) together with the full momentum vector and helicity for a charged-particle
/// track in the STAR solenoidal magnetic field.
class StHelixModel : public StTrackGeometry {
public:
    /// @brief Default constructor; initialises all parameters to zero.
    StHelixModel();
    /// @brief Construct with explicit helix parameters.
    /// @param q  Electric charge (+1 or -1).
    /// @param psi Azimuthal angle of momentum at origin (rad).
    /// @param c  Signed curvature (1/cm).
    /// @param dip Dip angle λ (rad).
    /// @param o  Origin (reference point) (cm).
    /// @param p  Momentum vector at origin (GeV/c).
    /// @param h  Helix sense (+1 or -1).
    StHelixModel(short q, float psi, float c, float dip,
                 const StThreeVectorF& o, const StThreeVectorF& p, short h);
    // StHelixModel(const StHelixModel&);            use default
    // StHelixModel& operator=(const StHelixModel&); use default
    ~StHelixModel();

    /// @brief Returns kHelixModel.
    StTrackModel           model() const;
    /// @brief Electric charge of the particle (+1 or -1).
    short                  charge() const;
    /// @brief Helix sense: +1 if the track turns clockwise viewed along +z.
    short                  helicity() const;
    /// @brief Signed curvature κ; |κ| = |qB|/p_T (1/cm).
    double                 curvature() const;
    /// @brief Azimuthal angle ψ of the track momentum at the origin (rad).
    double                 psi() const;
    /// @brief Dip angle λ = arctan(p_z/p_T) (rad).
    double                 dipAngle() const;
    /// @brief Reference point on the helix (cm).
    const StThreeVectorF&  origin() const;
    /// @brief Momentum vector at the origin (GeV/c).
    const StThreeVectorF&  momentum() const;
    /// @brief Physical helix parameterisation at the reference point.
    StPhysicalHelixD       helix() const;

    /// @brief Set the electric charge.
    void setCharge(short);
    /// @brief Set the helix sense.
    void setHelicity(short);
    /// @brief Set the signed curvature (1/cm).
    void setCurvature(double);
    /// @brief Set the azimuthal angle ψ of the momentum at the origin (rad).
    void setPsi(double);
    /// @brief Set the dip angle λ (rad).
    void setDipAngle(double);
    /// @brief Set the reference point on the helix (cm).
    void setOrigin(const StThreeVectorF&);
    /// @brief Set the momentum vector at the origin (GeV/c).
    void setMomentum(const StThreeVectorF&);
     
    /// @brief Virtual copy constructor; returns a heap-allocated copy.
    StTrackGeometry*       copy() const;     // virtual constructor
    
private:
    StTrackModel   mModel;     ///< Track geometry model identifier.
    Short_t        mCharge;    ///< Electric charge of the particle (+1 or -1).
    Float_t        mPsi;       ///< Azimuthal angle of the track momentum at the origin (rad).
    Float_t        mCurvature; ///< Signed curvature of the helix; |κ| = |qB|/p_T (1/cm).
    Float_t        mDipAngle;  ///< Dip angle λ = arctan(p_z/p_T) (rad).
    StThreeVectorF mOrigin;    ///< Reference point on the helix (cm).
    StThreeVectorF mMomentum;  ///< Momentum vector at the origin (GeV/c).
    Short_t        mHelicity;  ///< Helix sense: +1 if clockwise viewed along +z, -1 otherwise.
    
    ClassDef(StHelixModel,3)
};

#endif
