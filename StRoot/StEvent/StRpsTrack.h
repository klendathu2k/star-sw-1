/***************************************************************************
 * 
 * $Id: StRpsTrack.h,v 2.4 2015/10/22 20:31:31 ullrich Exp $
 *
 * Author: Rafal Sikora, 1 Oct 2015
 *
 ***************************************************************************
 *
 * Description: StRpsTrack class representing reconstructed track in 
 * the Roman Pot system, with all associated observables, such as 
 * momentum (px, py, pz) etc..
 *
 ***************************************************************************
 *
 * $Log: StRpsTrack.h,v $
 * Revision 2.4  2015/10/22 20:31:31  ullrich
 * StRpsTrack.cxx
 *
 * Revision 2.3  2015/10/08 20:53:34  ullrich
 * Changed comment of mBranch
 *
 * Revision 2.2  2015/10/07 17:30:13  ullrich
 * Changed const to enums and related changes.
 *
 * Revision 2.1  2015/10/02 19:48:14  ullrich
 * Initial Revision.
 *
 ***************************************************************************/
#ifndef StRpsTrack_hh
#define StRpsTrack_hh

/// @file StRpsTrack.h
/// @brief Reconstructed forward proton track in the Roman Pot System.

#include "StObject.h"
#include "StContainers.h"
#include "StThreeVectorF.hh"

class StRpsTrackPoint;

/// @brief Reconstructed forward proton track from the Roman Pot System (RPS).
///
/// A track is built from two local track points (one per RPS station in a detector
/// branch) and carries the reconstructed three-momentum and derived observables:
/// scattering angles theta and phi, azimuthal angle at the RP, fractional momentum
/// loss xi, Mandelstam variable t, and pseudorapidity.
class StRpsTrack : public StObject {
public:
    /// @brief Default constructor.
    StRpsTrack();
    /// @brief Copy constructor.
    StRpsTrack(const StRpsTrack&);
    ~StRpsTrack();

    /// @brief Assignment operator.
    StRpsTrack& operator=(const StRpsTrack&);
    /// @brief Track reconstruction type: local (single-station), global (two-station), or undefined.
    enum StRpsTrackType { rpsLocal, rpsGlobal, rpsUndefined };
    /// @brief Indices for the scattering angle components and their count.
    enum StRpsAngles { rpsAngleThetaX, rpsAngleThetaY, rpsAngleTheta, mNumberOfAngleTypes };

    /// @brief Returns the track point for the given station index (0 or 1) in the branch.
    StRpsTrackPoint* trackPoint(unsigned int) const;
    /// @brief Returns the reconstructed three-momentum vector (GeV/c).
    StThreeVectorF pVec() const;
    /// @brief Returns the detector branch index (EU=0, ED=1, WU=2, WD=3).
    int branch() const;
    /// @brief Returns the track reconstruction type.
    StRpsTrackType type() const;
    /// @brief Returns a bitmask indicating which detector planes contributed to this track.
    unsigned int planesUsed() const;
    
    /// @brief Returns the proton scattering angle (rad); component selected by @p angleType.
    double theta(unsigned int = rpsAngleTheta) const;
    /// @brief Returns the scattering angle measured at the Roman Pot (rad); component selected by @p angleType.
    double thetaRp(unsigned int = rpsAngleTheta) const;
    /// @brief Returns the azimuthal scattering angle phi (rad) from the momentum vector.
    double phi() const;
    /// @brief Returns the azimuthal angle phi (rad) measured at the Roman Pot.
    double phiRp() const;
    /// @brief Returns the Mandelstam variable t (GeV²) for the given beam momentum (GeV/c).
    double t(double) const;
    /// @brief Returns the fractional momentum loss xi = (p_beam - p) / p_beam for the given beam momentum.
    double xi(double) const;
    /// @brief Returns the magnitude of the reconstructed momentum (GeV/c).
    double p() const;
    /// @brief Returns the transverse momentum (GeV/c).
    double pt() const;
    /// @brief Returns the pseudorapidity of the reconstructed track.
    double eta() const;
    /// @brief Returns the time-of-flight of the proton (ns).
    double time() const;

    /// @brief Sets the track point for the given station index.
    void setTrackPoint(StRpsTrackPoint*, unsigned int);
    /// @brief Sets the reconstructed three-momentum vector (GeV/c).
    void setP(const StThreeVectorF&);
    /// @brief Sets the detector branch index.
    void setBranch(int);
    /// @brief Sets the track reconstruction type.
    void setType(StRpsTrackType);

    /// @brief Number of RPS stations contributing to a single branch track.
    enum {mNumberOfStationsInBranch = 2};

private:
    StPtrVecRpsTrackPoint mTrackPoints; ///< Pointers to the two local track points (one per RPS station in branch).
    StThreeVectorF mP;                  ///< Reconstructed three-momentum vector (GeV/c).
    Int_t          mBranch;             ///< Detector branch index: EU=0, ED=1, WU=2, WD=3.
    StRpsTrackType mType;               ///< Track reconstruction type (local or global).
    
    ClassDef(StRpsTrack, 1)
};

inline StRpsTrackPoint* StRpsTrack::trackPoint(unsigned int station) const
{
    return station < mNumberOfStationsInBranch ? mTrackPoints[station] : nullptr;
}
inline StThreeVectorF StRpsTrack::pVec() const { return mP; }
inline int StRpsTrack::branch() const { return mBranch; }
inline StRpsTrack::StRpsTrackType StRpsTrack::type() const { return mType; }
inline double StRpsTrack::phi() const { return mP.phi(); }
inline double StRpsTrack::t(double beamMomentum) const
{
  return -2*beamMomentum*beamMomentum*(1-xi(beamMomentum))*(1-cos(theta(rpsAngleTheta)));
}
inline double StRpsTrack::xi(double beamMomentum) const
{
    return (beamMomentum - mP.mag())/beamMomentum;
}
inline double StRpsTrack::p() const { return mP.mag(); }
inline double StRpsTrack::pt() const { return mP.perp(); }
inline double StRpsTrack::eta() const { return mP.pseudoRapidity(); }

inline void StRpsTrack::setTrackPoint(StRpsTrackPoint* trackPoint, unsigned int station)
{
    if (station<mNumberOfStationsInBranch)
        mTrackPoints[station] = trackPoint;
}
inline void StRpsTrack::setP(const StThreeVectorF& P) { mP = P; }
inline void StRpsTrack::setBranch(int branch) { mBranch = branch; }
inline void StRpsTrack::setType(StRpsTrack::StRpsTrackType type) { mType = type; }

#endif
