/*!
 * \class StRichCollection 
 * \author Brian Lasiuk, May 2000
 *
 *    Persistent data which is written into StEvent
 *    directly from the reco chain
 *
 */
/***************************************************************************
 *
 * $Id: StRichCollection.h,v 2.4 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: Brian Lasiuk, May 2000
 ***************************************************************************
 *
 * Description:
 *   Persistent data which is written into StEvent
 *   directly from the reco chain
 *
 ***************************************************************************
 *
 * $Log: StRichCollection.h,v $
 * Revision 2.4  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:39  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2001/02/22 21:04:17  lasiuk
 * keep the tracks that fly through the RICH in
 * the collection
 *
 * Revision 2.1  2000/05/22 21:48:17  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StRichCollection_hh
#define StRichCollection_hh

/// @file StRichCollection.h
/// @brief Container for all persistent RICH detector data in a single event.

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StRichCluster.h"
#include "StRichHit.h"

/// @brief Container for all persistent RICH detector data written into StEvent.
class StRichCollection : public StObject {
public:
    /// @brief Default constructor.
    StRichCollection();
    ~StRichCollection();
//     StRichCollection(const StRichCollection&) { /* nopt */ }
//     StRichCollection& operator=(const StRichCollection&) {/* use default */}

    /// @brief Returns a const reference to the vector of raw RICH pixels.
    const StSPtrVecRichPixel&    getRichPixels() const;
    /// @brief Returns a mutable reference to the vector of raw RICH pixels.
    StSPtrVecRichPixel&          getRichPixels();

    /// @brief Returns a const reference to the vector of RICH pad clusters.
    const StSPtrVecRichCluster&  getRichClusters() const;
    /// @brief Returns a mutable reference to the vector of RICH pad clusters.
    StSPtrVecRichCluster&        getRichClusters();

    /// @brief Returns a const reference to the vector of reconstructed RICH hits.
    const StSPtrVecRichHit&      getRichHits() const;
    /// @brief Returns a mutable reference to the vector of reconstructed RICH hits.
    StSPtrVecRichHit&            getRichHits();

    /// @brief Returns a const reference to the vector of tracks traversing the RICH.
    const StPtrVecTrack&         getTracks() const;
    /// @brief Returns a mutable reference to the vector of tracks traversing the RICH.
    StPtrVecTrack&               getTracks();

    /// @brief Adds a raw pixel to the collection.
    void addPixel(const StRichPixel*);
    /// @brief Adds a pad cluster to the collection.
    void addCluster(const StRichCluster*);
    /// @brief Adds a reconstructed hit to the collection.
    void addHit(const StRichHit*);
    /// @brief Adds a track associated with the RICH to the collection.
    void addTrack(const StTrack*);

    /// @brief Returns true if the pixel vector is non-empty.
    bool pixelsPresent()   const;
    /// @brief Returns true if the cluster vector is non-empty.
    bool clustersPresent() const;
    /// @brief Returns true if the hit vector is non-empty.
    bool hitsPresent()     const;

private:
    StSPtrVecRichPixel     mRichPixels;    ///< Collection of raw RICH pad pixels
    StSPtrVecRichCluster   mRichClusters;  ///< Collection of RICH pad clusters
    StSPtrVecRichHit       mRichHits;      ///< Collection of reconstructed RICH hits

    StPtrVecTrack          mTracks;        ///< Tracks traversing the RICH detector

    ClassDef(StRichCollection, 1)
};
#endif
