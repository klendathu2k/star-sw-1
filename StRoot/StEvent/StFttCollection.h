/***************************************************************************
 *
 * $Id: StFttCollection.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: Akio Ogawa, 2018 Aug
 ***************************************************************************
 *
 * Description:
 * Collection of all hits (towers), clusters and points (photons) in the FCS
 * This collection owns all these objects, and is itself owned by StEvent.
 * It is therefore vital to *not* delete any of the objects stored in this
 * container yourself - the collection will handle freeing memory.
 * Similarly, any object added to the collection via an add() method must be
 * allocated with new, and not be owned anywhere else.
 *
 ***************************************************************************/
#ifndef StFttCollection_hh
#define StFttCollection_hh

/// @file StFttCollection.h
/// @brief Event-level container for all Forward sTGC Tracker (FTT) data in StEvent.

#include "Stiostream.h"
#include "StObject.h"
#include "StEnumerations.h"
#include "StContainers.h"

class StFttRawHit;
class StFttCluster;
class StFttPoint;

/// @brief Top-level container for all FTT raw hits, strip clusters, and reconstructed points in one event.
///
/// Owns all stored objects; do not delete pointers retrieved from this class.
/// All objects added via @c add*() must be heap-allocated with @c new.
class StFttCollection : public StObject {
public:
    StFttCollection();
    ~StFttCollection();
    
    void addRawHit(StFttRawHit*);            // Add a hit 
    StSPtrVecFttRawHit& rawHits();             // Return the hit list
    const StSPtrVecFttRawHit& rawHits() const; // Return the hit list
    unsigned int numberOfRawHits() const;   // Return the number of hits

    void addCluster(StFttCluster*);            // Add a cluster
    StSPtrVecFttCluster& clusters();             // Return the cluster list
    const StSPtrVecFttCluster& clusters() const; // Return the cluster list
    unsigned int numberOfClusters() const;       // Return the number of clusters

    void addPoint(StFttPoint*);              // Add a point
    StSPtrVecFttPoint& points();             // Return the point list
    const StSPtrVecFttPoint& points() const; // Return the point list
    unsigned int numberOfPoints() const;     // Return the number of points

    void print(int option=1);
    
private:
    StSPtrVecFttRawHit  mRawHits;   ///< All FTT raw strip hits in the event.
    StSPtrVecFttCluster mClusters;  ///< All FTT strip clusters in the event.
    StSPtrVecFttPoint   mPoints;    ///< All FTT reconstructed space points in the event.

    ClassDef(StFttCollection,1)

};

#endif
