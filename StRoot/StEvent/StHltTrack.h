/***************************************************************************
 *
 * $Id: StHltTrack.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltTrack.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltTrack_hh
#define StHltTrack_hh

/// @file StHltTrack.h
/// @brief HLT reconstructed track (global or primary) produced by the High Level Trigger.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"
#include "StEnumerations.h"

class StHltTrackNode;

/// @brief HLT reconstructed track storing helix parameters, dE/dx, and fit quality from online tracking.
class StHltTrack : public StObject {
public:
    StHltTrack();
    virtual ~StHltTrack();

    /// @brief Track type (global or primary).
    StTrackType type() const;
    /// @brief Unique track identifier (primary key).
    int id() const;
    /// @brief Track quality and status flags.
    unsigned int flag() const;
    /// @brief Innermost TPC pad row used in the track fit.
    int innerMostRow() const;
    /// @brief Outermost TPC pad row used in the track fit.
    int outerMostRow() const;
    /// @brief Number of TPC hits on the track.
    unsigned int nHits() const;
    /// @brief Number of hits used in the dE/dx calculation.
    unsigned int ndedx() const;
    /// @brief Electric charge of the track (+1 or -1).
    int q() const;
    /// @brief Chi-squared of the momentum fit (index 0 or 1).
    float chi2(int) const;
    /// @brief Mean dE/dx energy loss (GeV/cm).
    float dedx() const;
    /// @brief Transverse momentum at the reference point (GeV/c).
    float pt() const;
    /// @brief Azimuthal angle φ₀ of the reference point on the helix (rad).
    float phi0() const;
    /// @brief Azimuthal angle ψ of the momentum at the reference point (rad).
    float psi() const;
    /// @brief Radial coordinate r of the helix reference point (cm).
    float r0() const;
    /// @brief Tangent of the dip angle (tan λ) at the reference point.
    float tanl() const;
    /// @brief z coordinate of the helix reference point (cm).
    float z0() const;
    /// @brief Track arc length from the first to last hit (cm).
    float length() const;
    /// @brief Uncertainty on transverse momentum (GeV/c).
    float dpt() const;
    /// @brief Uncertainty on the azimuthal angle ψ (rad).
    float dpsi() const;
    /// @brief Uncertainty on z₀ (cm).
    float dz0() const;
    /// @brief Uncertainty on tan λ.
    float dtanl() const;

    /// @brief Associated HLT track node.
    StHltTrackNode* trackNode();
    const StHltTrackNode* trackNode() const;

    /// @brief Set the track type.
    void setType(StTrackType);
    /// @brief Set the track identifier.
    void setId(int);
    /// @brief Set the track quality flags.
    void setFlag(unsigned short);
    /// @brief Set the innermost TPC pad row.
    void setInnerMostRow(char);
    /// @brief Set the outermost TPC pad row.
    void setOuterMostRow(char);
    /// @brief Set the number of TPC hits.
    void setNHits(unsigned char);
    /// @brief Set the number of hits used for dE/dx.
    void setNDedx(unsigned char);
    /// @brief Set the track charge.
    void setQ(char);
    /// @brief Set a chi-squared component (index 0 or 1).
    void setChi2(int,float);
    /// @brief Set the mean dE/dx (GeV/cm).
    void setDedx(float);
    /// @brief Set the transverse momentum (GeV/c).
    void setPt(float);
    /// @brief Set the azimuthal angle φ₀ of the reference point (rad).
    void setPhi0(float);
    /// @brief Set the azimuthal angle ψ of the momentum (rad).
    void setPsi(float);
    /// @brief Set the radial reference coordinate r₀ (cm).
    void setR0(float);
    /// @brief Set the tangent of the dip angle.
    void setTanl(float);
    /// @brief Set the z reference coordinate z₀ (cm).
    void setZ0(float);
    /// @brief Set the track arc length (cm).
    void setLength(float);
    /// @brief Set the uncertainty on pT (GeV/c).
    void setDpt(float);
    /// @brief Set the uncertainty on ψ (rad).
    void setDpsi(float);
    /// @brief Set the uncertainty on z₀ (cm).
    void setDz0(float);
    /// @brief Set the uncertainty on tan λ.
    void setDtanl(float);

    /// @brief Set the associated track node.
    void setTrackNode(StHltTrackNode*);
    
protected:
    StTrackType mType;    ///< Track type (global or primary).
    int mId;              ///< Unique track identifier (primary key).
    unsigned short mFlag; ///< Track quality and status flags.
    char mInnerMostRow;   ///< Innermost TPC pad row used in the fit.
    char mOuterMostRow;   ///< Outermost TPC pad row used in the fit.
    unsigned char mNHits; ///< Number of TPC hits on the track.
    unsigned char mNDedx; ///< Number of hits used in the dE/dx calculation.
    char mQ;              ///< Electric charge (+1 or -1).
    float mChi2[2];       ///< Chi-squared components of the momentum fit.
    float mDedx;          ///< Mean dE/dx energy loss (GeV/cm).
    float mPt;            ///< Transverse momentum at the reference point (GeV/c).
    float mPhi0;          ///< Azimuthal angle φ₀ of the helix reference point (rad).
    float mPsi;           ///< Azimuthal angle ψ of the momentum at the reference point (rad).
    float mR0;            ///< Radial coordinate r₀ of the helix reference point (cm).
    float mTanl;          ///< Tangent of the dip angle (tan λ) at the reference point.
    float mZ0;            ///< z coordinate of the helix reference point (cm).
    float mLength;        ///< Track arc length from first to last hit (cm).
    float mDpt;           ///< Uncertainty on pT (GeV/c).
    float mDpsi;          ///< Uncertainty on ψ (rad).
    float mDz0;           ///< Uncertainty on z₀ (cm).
    float mDtanl;         ///< Uncertainty on tan λ.
    
#ifdef __CINT__
    StObjLink mTrackNode;
#else
    StLink<StHltTrackNode> mTrackNode;
#endif //__CINT__
    
    ClassDef(StHltTrack,1)
};

inline StTrackType StHltTrack::type() const {return mType;}
inline int StHltTrack::id() const {return mId;}
inline unsigned int StHltTrack::flag() const {return mFlag;}
inline int StHltTrack::innerMostRow() const {return mInnerMostRow;}
inline int StHltTrack::outerMostRow() const {return mOuterMostRow;}
inline unsigned int StHltTrack::nHits() const {return mNHits;}
inline unsigned int StHltTrack::ndedx() const {return mNDedx;}
inline int StHltTrack::q() const {return mQ;}
inline float StHltTrack::chi2(int i) const { if (i==0 || i==1) return mChi2[i]; else return 0;}
inline float StHltTrack::dedx() const {return mDedx;}
inline float StHltTrack::pt() const {return mPt;}
inline float StHltTrack::phi0() const {return mPhi0;}
inline float StHltTrack::psi() const {return mPsi;}
inline float StHltTrack::r0() const {return mR0;}
inline float StHltTrack::tanl() const {return mTanl;}
inline float StHltTrack::z0() const {return mZ0;}
inline float StHltTrack::length() const {return mLength;}
inline float StHltTrack::dpt() const {return mDpt;}
inline float StHltTrack::dpsi() const {return mDpsi;}
inline float StHltTrack::dz0() const {return mDz0;}
inline float StHltTrack::dtanl() const {return mDtanl;}

inline StHltTrackNode* StHltTrack::trackNode() {return mTrackNode;}
inline const StHltTrackNode* StHltTrack::trackNode() const {return mTrackNode;}


ostream& operator<<(ostream&, const StHltTrack&); ///< Printting operator

#endif







