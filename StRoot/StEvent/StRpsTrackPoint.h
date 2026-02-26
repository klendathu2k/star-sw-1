/***************************************************************************
 *
 * $Id: StRpsTrackPoint.h,v 2.2 2015/10/07 17:30:13 ullrich Exp $
 *
 * Author: Rafal Sikora, 1 Oct 2015
 *
 ***************************************************************************
 *
 * Description: StRpsTrackPoint class representing reconstructed (x,y,z)
 * position of the hit in single Roman Pot detector
 *
 ***************************************************************************
 *
 * $Log: StRpsTrackPoint.h,v $
 * Revision 2.2  2015/10/07 17:30:13  ullrich
 * Changed const to enums and related changes.
 *
 * Revision 2.1  2015/10/02 19:48:14  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StRpsTrackPoint_hh
#define StRpsTrackPoint_hh

/// @file StRpsTrackPoint.h
/// @brief Reconstructed hit position in a single Roman Pot detector station.

#include "StObject.h"
#include "StThreeVectorF.hh"


/// @brief Reconstructed (x, y, z) hit position in a single Roman Pot detector station.
///
/// A track point is built from the cluster positions in the four silicon strip planes
/// of one Roman Pot. It also stores the PMT timing signals and a quality classification.
class StRpsTrackPoint : public StObject {
public:
    /// @brief Default constructor.
    StRpsTrackPoint();
    /// @brief Copy constructor.
    StRpsTrackPoint(const StRpsTrackPoint&);
    ~StRpsTrackPoint();
    
    /// @brief Assignment operator.
    StRpsTrackPoint& operator=(const StRpsTrackPoint&);
    /// @brief Quality of the track point reconstruction.
    enum StRpsTrackPointQuality {rpsNormal, rpsGolden, rpsNotSet};
    
    /// @brief Returns the reconstructed hit position as a three-vector (cm).
    StThreeVectorF positionVec() const;
    /// @brief Returns the Roman Pot identifier for this track point.
    int rpId() const;
    /// @brief Returns the cluster index in the given detector plane (0–3) contributing to this point.
    int clusterId(unsigned int) const;
    /// @brief Returns the PMT timing signal (ns) for the given PMT index (0–1).
    double time(unsigned int) const;
    /// @brief Returns the reconstruction quality of this track point.
    StRpsTrackPointQuality quality() const;
    /// @brief Returns a bitmask of the planes used in reconstructing this track point.
    unsigned int planesUsed() const;
    
    /// @brief Returns the x-coordinate of the reconstructed hit position (cm).
    double x() const;
    /// @brief Returns the y-coordinate of the reconstructed hit position (cm).
    double y() const;
    /// @brief Returns the z-coordinate of the reconstructed hit position (cm).
    double z() const;
    
    /// @brief Sets the reconstructed hit position (cm).
    void setPosition(const StThreeVectorF&);
    /// @brief Sets the Roman Pot identifier.
    void setRpId(int);
    /// @brief Sets the cluster index for the given detector plane.
    void setClusterId(int, unsigned int);
    /// @brief Sets the PMT timing signal (ns) for the given PMT index.
    void setTime(double, unsigned int);
    /// @brief Sets the reconstruction quality of this track point.
    void setQuality(StRpsTrackPointQuality);

    /// @brief Number of PMT timing detectors per Roman Pot, and number of silicon planes per Roman Pot.
    enum {mNumberOfPmtsInRp = 2, mNumberOfPlanesInRp = 4};
    
private:
    int mRpId;                          ///< Roman Pot identifier for this track point.
    int mClusterId[mNumberOfPlanesInRp]; ///< Cluster index in each detector plane contributing to this point.
    double mTime[mNumberOfPmtsInRp];    ///< PMT timing signals (ns) for the two scintillator detectors.

    StThreeVectorF         mPosition; ///< Reconstructed (x, y, z) hit position (cm).
    StRpsTrackPointQuality mQuality;  ///< Reconstruction quality classification.

    ClassDef( StRpsTrackPoint, 1 )
};

inline StThreeVectorF StRpsTrackPoint::positionVec() const { return mPosition; }
inline int StRpsTrackPoint::rpId() const { return mRpId; }
inline int StRpsTrackPoint::clusterId(unsigned int planeId ) const
{
    return planeId<mNumberOfPlanesInRp ? mClusterId[planeId] : -1;
}
inline double StRpsTrackPoint::time(unsigned int pmtId) const
{
    return pmtId<mNumberOfPmtsInRp ? mTime[pmtId] : -1;
}
inline StRpsTrackPoint::StRpsTrackPointQuality StRpsTrackPoint::quality() const { return mQuality; }
inline double StRpsTrackPoint::x() const { return mPosition.x(); }
inline double StRpsTrackPoint::y() const { return mPosition.y(); }
inline double StRpsTrackPoint::z() const { return mPosition.z(); }

inline void StRpsTrackPoint::setPosition(const StThreeVectorF& position)
{
    mPosition = position;
}
inline void StRpsTrackPoint::setRpId(int rpId) { mRpId = rpId; }
inline void StRpsTrackPoint::setClusterId(int clusterId, unsigned int planeId)
{
    if( planeId<mNumberOfPlanesInRp )
        mClusterId[planeId] = clusterId;
}
inline void StRpsTrackPoint::setTime(double timeVal, unsigned int pmtId)
{
    if( pmtId<mNumberOfPmtsInRp ) mTime[pmtId] = timeVal;
}
inline void StRpsTrackPoint::setQuality(StRpsTrackPoint::StRpsTrackPointQuality quality )
{
    mQuality = quality;
}

#endif
