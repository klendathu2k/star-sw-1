/***************************************************************************
 *
 * $Id: StFwdTrackCollection.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: jdb, Feb 2022
 ***************************************************************************
 *
 * Description:
 *
 **************************************************************************/
#ifndef StFwdTrackCollection_hh
#define StFwdTrackCollection_hh

/// @file StFwdTrackCollection.h
/// @brief Event-level collection of forward-rapidity tracks stored in StEvent.

#include "StObject.h"
#include "StContainers.h"

class StFwdTrack;

/// @brief Top-level container for all reconstructed StFwdTrack objects in one event.
class StFwdTrackCollection : public StObject {
public:
    StFwdTrackCollection();
    ~StFwdTrackCollection();

    void addTrack(StFwdTrack*);              // Add a track
    StSPtrVecFwdTrack& tracks();             // Return the track list
    const StSPtrVecFwdTrack& tracks() const; // Return the track list
    unsigned int numberOfTracks() const;     // Return the number of tracks

private:
    StSPtrVecFwdTrack   mTracks;   ///< Persistent vector of reconstructed forward tracks.

    ClassDef(StFwdTrackCollection,1)

};

#endif
