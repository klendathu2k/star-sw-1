/***************************************************************************
 *
 * $Id: StRpsCollection.h,v 2.3 2015/10/02 19:50:50 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StRpsCollection.h,v $
 * Revision 2.3  2015/10/02 19:50:50  ullrich
 * Added containers for tracks and points.
 *
 * Revision 2.2  2010/02/04 18:16:09  ullrich
 * Added new member mSiliconBunch and referring access methods.
 *
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRpsCollection_hh
#define StRpsCollection_hh

/// @file StRpsCollection.h
/// @brief Top-level container for Roman Pot System data in a single event.

#include "StObject.h"
#include "StContainers.h"
#include "StRpsRomanPot.h"
#include "StRpsTrackPoint.h"
#include "StRpsTrack.h"

/// @brief Top-level event data container for the Roman Pot System (RPS).
///
/// Holds the array of eight Roman Pot detectors and the associated reconstructed
/// track points and global forward proton tracks for a single STAR event.
class StRpsCollection : public StObject {
public:
    /// @brief Default constructor.
    StRpsCollection();
    ~StRpsCollection();
    
    /// @brief Returns the total number of Roman Pot detectors (always 8).
    unsigned int numberOfRomanPots() const;
    
    /// @brief Returns a const pointer to the Roman Pot at the given index.
    const StRpsRomanPot* romanPot(unsigned int) const;
    /// @brief Returns a pointer to the Roman Pot at the given index.
    StRpsRomanPot* romanPot(unsigned int);
    
    /// @brief Returns a flat collection of all clusters from all Roman Pot planes.
    StPtrVecRpsCluster clusters() const;
    /// @brief Returns the collection of reconstructed track points.
    StPtrVecRpsTrackPoint trackPoints() const;
    /// @brief Returns the collection of reconstructed forward proton tracks.
    StPtrVecRpsTrack tracks() const;
    /// @brief Returns the silicon detector bunch crossing number.
    unsigned char siliconBunch() const;
    
    /// @brief Sets the silicon detector bunch crossing number.
    void setSiliconBunch(unsigned char);
    /// @brief Adds a reconstructed track point to the collection.
    void addTrackPoint(const StRpsTrackPoint*);
    /// @brief Adds a reconstructed forward proton track to the collection.
    void addTrack(const StRpsTrack*);
    
    /// @brief Total number of Roman Pot detector stations in the RPS.
    enum {mNumberOfRomanPots = 8};
    
private:
    StRpsRomanPot mRomanPots[mNumberOfRomanPots]; ///< Array of all eight Roman Pot detector stations.
    UChar_t mSiliconBunch;                        ///< Silicon detector bunch crossing number.
    
    StSPtrVecRpsTrackPoint mTrackPoints; ///< Reconstructed track points (one per Roman Pot station).
    StSPtrVecRpsTrack mTracks;           ///< Reconstructed forward proton tracks.
    
    ClassDef(StRpsCollection, 3)
};


inline void StRpsCollection::addTrackPoint(const StRpsTrackPoint *trackPoint) {
    mTrackPoints.push_back(trackPoint);
}
inline void StRpsCollection::addTrack(const StRpsTrack *track) {
    mTracks.push_back(track);
}

#endif

