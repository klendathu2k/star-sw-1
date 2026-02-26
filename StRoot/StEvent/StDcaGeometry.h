/*!
 * \class StDcaGeometry
 * \author Victor Perevoztchikov, Thomas Ullrich, May 2006
 */
/***************************************************************************
 *
 * $Id: StDcaGeometry.h,v 2.8 2017/06/01 23:48:44 smirnovd Exp $
 *
 * Author: Victor Perevoztchikov, Thomas Ullrich, May 2006
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StDcaGeometry.h,v $
 * Revision 2.8  2017/06/01 23:48:44  smirnovd
 * [Cosmetic] StDcaGeometry: Whitespace adjustments
 *
 * Revision 2.7  2012/05/07 14:42:57  fisyak
 * Add handilings for Track to Fast Detectors Matching
 *
 * Revision 2.6  2012/03/28 13:39:46  fisyak
 * Add default parameter for Print
 *
 * Revision 2.5  2010/01/26 20:34:39  fisyak
 * Add print out and  conversion from DCA to x,y,z,px,py,pz
 *
 * Revision 2.4  2009/10/28 13:54:35  fisyak
 * Forgot one more set
 *
 * Revision 2.3  2008/03/04 01:03:14  perev
 * remove redundant mHz
 *
 * Revision 2.2  2006/08/04 19:08:43  perev
 * CleanUpOnly
 *
 * Revision 2.1  2006/05/24 17:27:43  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StDcaGeometry_hh
#define StDcaGeometry_hh
/// @file StDcaGeometry.h
/// @brief Track geometry (helix parameters) at the point of closest approach to the beam line.

#include "StObject.h"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "THelixTrack.h"

/// @brief Track geometry (helix parameters) at the point of closest approach to the beam line.
///
/// StDcaGeometry stores the compact five-parameter representation of a global
/// track's helix at the beam-line DCA: signed impact parameter, z-coordinate,
/// azimuthal angle ψ, signed inverse p_T, and tangent of the dip angle λ.
/// The covariance of these five parameters is stored as a packed 15-element
/// upper-triangle matrix.
class StDcaGeometry : public StObject {
public:
    /// @brief Default constructor; initialises all parameters to zero.
    StDcaGeometry();
    virtual ~StDcaGeometry();

    /// @brief Electric charge (+1 or -1); derived from the sign of the inverse p_T.
    Int_t            charge()    const {return (mPti>0)? -1:1;}  // synchro with StiTrackNode charge definition
    /// @brief Signed impact parameter d_0 (cm); x = -d_0·sin ψ, y = d_0·cos ψ.
    Double_t         impact()    const {return mImp;}
    /// @brief Signed curvature κ = q·B·c / p_T (1/cm).
    Double_t         curvature() const {return mCurv;}
    /// @brief Azimuthal angle ψ of the track momentum at the DCA point (rad).
    Double_t         psi()       const {return mPsi ;}
    /// @brief Dip angle λ = arctan(p_z/p_T) at the DCA point (rad).
    Double_t         dipAngle()  const {return atan(mTan);}
    /// @brief Tangent of the dip angle λ (= p_z/p_T).
    Double_t         tanDip()    const {return mTan ;}
    /// @brief Transverse momentum at the DCA point (GeV/c).
    Double_t         pt()        const {return 1./fabs(mPti);}
    /// @brief z-coordinate of the track at the beam-line DCA (cm).
    Double_t         z()         const {return mZ   ;}
    /// @brief Curvature normalised by the signed inverse p_T (used for helix construction).
    Double_t         hz()        const {return mCurv/mPti;}
    /// @brief 3-D position at the DCA point (cm).
    StThreeVectorF   origin()    const;
    /// @brief Momentum vector at the DCA point (GeV/c).
    StThreeVectorF   momentum()  const;
    /// @brief Physical helix parameterisation at the DCA point.
    StPhysicalHelixD helix()     const;
    /// @brief THelixTrack representation at the DCA point.
    THelixTrack      thelix()    const;
    /// @brief Pointer to the raw parameter array [imp, z, psi, pti, tan, curv].
    const float*     params()    const {return &mImp;}
    /// @brief Pointer to the raw 15-element upper-triangle error matrix.
    const float*     errMatrix() const {return &mImpImp;}
    /// @brief Convert DCA parameters to Cartesian position and momentum with covariance.
    void GetXYZ(Double_t xyzp[6], Double_t CovXyzp[21]) const;
    virtual void     Print(Option_t *option = "") const;
    //
    // Experts only set function
    //
    /// @brief Set all DCA parameters and their covariance matrix (float version).
    void set(const Float_t pars[6], const Float_t errs[15]);
    /// @brief Set all DCA parameters and their covariance matrix (double version).
    void set(const Double_t pars[6], const Double_t errs[15]);

private:
    Char_t  mBeg[1];//!
    /// signed impact parameter; Signed in such a way that:
    ///     x =  -impact*sin(Psi)
    ///     y =   impact*cos(Psi)
    Float_t  mImp;
    ///  Z-coordinate of this track (reference plane)
    Float_t  mZ;
    ///  Psi angle of the track
    Float_t  mPsi;
    /// signed invert pt [sign = sign(-qB)]
    Float_t  mPti;
    /// tangent of the track momentum dip angle
    Float_t  mTan;
    /// signed curvature
    Float_t  mCurv;
    
    /// pars errors
    Float_t  mImpImp;
    Float_t  mZImp,   mZZ;
    Float_t  mPsiImp, mPsiZ, mPsiPsi;
    Float_t  mPtiImp, mPtiZ, mPtiPsi, mPtiPti;
    Float_t  mTanImp, mTanZ, mTanPsi, mTanPti, mTanTan;
    Char_t   mEnd[1];//!
    
    ClassDef(StDcaGeometry,3)
};
ostream&  operator<<(ostream& os, StDcaGeometry const & dca);

#endif
