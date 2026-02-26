/*!
 * \class StExtGeometry
 * \author Victor Perevoztchikov,  November 2016
 */
/***************************************************************************
 *
 * $Id: StExtGeometry.h,v 2.3 2017/05/04 00:56:18 perev Exp $
 *
 * Author: Victor Perevoztchikov,  November  2016
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Id: StExtGeometry.h,v 2.3 2017/05/04 00:56:18 perev Exp $
 **************************************************************************/
#ifndef StExtGeometry_hh
#define StExtGeometry_hh
/// @file StExtGeometry.h
/// @brief Extended track geometry (helix state) at a named external detector surface.

#include "StObject.h"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "THelixTrack.h"

/// @brief Extended track geometry (helix state) extrapolated to a named external detector surface.
///
/// StExtGeometry stores the track state (position, helix parameters, and error matrix)
/// at the point where the helix intersects a specific detector surface (e.g. BEMC,
/// BTOF).  Multiple instances are linked in a singly-linked list via mNext and
/// attached to a StTrack via StTrack::extGeometry().
class StExtGeometry : public StObject {
public:
    friend class StTrack;

    /// @brief Construct with the name of the external detector surface.
    StExtGeometry(const char *name="");
    virtual ~StExtGeometry();
    
    /// @brief Name of the external detector surface (e.g. "BEMC", "BTOF").
    const char *name() const;
    /// @brief Electric charge (+1 or -1); derived from the sign of the inverse p_T.
    int        charge() const;
    /// @brief Radial distance of the extrapolation point in the xy-plane (cm).
    double     rxy()       const;
    /// @brief Azimuthal angle of the extrapolation point (rad).
    double     phi()       const;
    /// @brief z-coordinate of the extrapolation point (cm).
    double     z()         const;
    /// @brief Signed curvature of the track at this surface (1/cm).
    double     curvature() const;
    /// @brief Azimuthal angle ψ of the track momentum at this surface (rad).
    double     psi()       const;
    /// @brief Tangent of the dip angle λ (= p_z/p_T) at this surface.
    double     tanDip()    const;
    /// @brief Transverse momentum at this surface (GeV/c).
    double     pt()        const;
    /// @brief Curvature normalised by the signed inverse p_T.
    double     hz()        const;
    /// @brief Total arc length from the primary vertex to this surface (cm).
    double     length()    const;
    /// @brief Signed curvature (alias for curvature()).
    double     curve()     const;
    
    /// @brief 3-D Cartesian position at the extrapolation surface (cm).
    StThreeVectorF   origin()    const;
    /// @brief Momentum vector at the extrapolation surface (GeV/c).
    StThreeVectorF   momentum()  const;
    
    /// @brief Physical helix parameterisation at this surface.
    StPhysicalHelixD helix()     const;
    /// @brief THelixTrack representation at this surface.
    THelixTrack      thelix()    const;
    /// @brief Pointer to the raw parameter array [phi, z, psi, pti, tan, curv, len].
    const float*     params()    const;
    /// @brief Pointer to the raw 15-element upper-triangle error matrix.
    const float*     errMatrix() const;
    //
    // Experts only set function
    //
    /// @brief Set the detector surface name.
    void setName(const char *name);
    /// @brief Set all track parameters and error matrix at radial distance rXY.
    void set(double rXY,const double pars[6], const double errs[15]);
    /// @brief Set the total arc length to this surface (cm).
    void setLength(double len);
    /// @brief Set the signed curvature.
    void setCurve (double cur);
    
    /// @brief Prepend this instance to the linked list headed by *top.
    void add(StExtGeometry **top);
    /// @brief Next StExtGeometry in the linked list (null if last).
    const StExtGeometry* next() const;
    
    /// @brief Parameter indices: phi, z, psi, 1/pt, tan(dip), curvature, arc length.
    enum {kPhi,kZ,kPsi,kPti,kTan,kCurv,kLen};
    /// @brief Error-matrix element indices for the upper triangle.
    enum {kPhiPhi
        ,kPhiZ,   kZZ
        ,kPhiPsi, kZPsi, kPsiPsi
        ,kPhiPti, kZPti, kPsiPti, kPtiPti
        ,kPhiTan, kZTan, kPsiTan, kPtiTan, kTanTan};
    
protected:
    char     mName[8];      ///< Name of the external detector surface (null-terminated, max 7 chars).
    StExtGeometry *mNext;   ///< Next node in the singly-linked list of extended geometries.
    
    Float_t  mRxy;  ///< Radial distance of the extrapolation point in the xy-plane (cm).
    Float_t  mPhi;  ///< Azimuthal angle of the extrapolation point (rad).
    Float_t  mZ;    ///< z-coordinate of the extrapolation point (cm).
    Float_t  mPsi;  ///< Azimuthal angle of the track momentum at this surface (rad).
    Float_t  mPti;  ///< Signed inverse transverse momentum; sign = sign(-qB) (GeV/c)^{-1}.
    Float_t  mTan;  ///< Tangent of the dip angle λ (= p_z/p_T).
    Float_t  mCurv; ///< Signed curvature; sign = sign(-qB) (1/cm).
    Float_t  mLen;  ///< Total arc length from the primary vertex to this surface (cm).
    
    Float_t  mG[15]; ///< Upper-triangle of the 6×6 track-parameter error matrix (15 elements).
    char     mEnd[1];	//!
    
    ClassDef(StExtGeometry,2)
};

inline const char* StExtGeometry::name() const {return mName;}
inline int StExtGeometry::charge() const {return (mPti>0)? -1:1;}
inline double StExtGeometry::rxy() const {return mRxy;}
inline double StExtGeometry::phi() const {return mPhi ;}
inline double StExtGeometry::z() const {return mZ   ;}
inline double StExtGeometry::curvature() const {return mCurv;}
inline double StExtGeometry::psi() const {return mPsi ;}
inline double StExtGeometry::tanDip() const {return mTan ;}
inline double StExtGeometry::pt() const {return 1./fabs(mPti);}
inline double StExtGeometry::hz() const {return mCurv/mPti;}
inline double StExtGeometry::length() const {return mLen;}
inline double StExtGeometry::curve() const {return mCurv;}
inline const float* StExtGeometry::params() const {return &mPhi;}
inline const float*  StExtGeometry::errMatrix() const {return mG;}
inline void StExtGeometry::setLength(double len){mLen  = len;}
inline void StExtGeometry::setCurve (double cur){mCurv = cur;}
inline const StExtGeometry* StExtGeometry::next() const { return mNext;}

#endif
