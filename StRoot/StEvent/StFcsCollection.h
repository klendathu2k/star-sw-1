/***************************************************************************
 *
 * $Id: StFcsCollection.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
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
 ***************************************************************************
 *
 * $Log: StFcsCollection.h,v $
 * Revision 2.1  2021/01/11 20:25:37  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFcsCollection_hh
#define StFcsCollection_hh

/// @file StFcsCollection.h
/// @brief Event-level collection of all FCS hits, clusters, and photon points.

#include "Stiostream.h"
#include "StObject.h"
#include "StEnumerations.h"
#include "StContainers.h"

class StFcsHit;
class StFcsCluster;
class StFcsPoint;
//class StFcsPointPair;

/// @brief Container owning all FCS hits, clusters, and photon points for one event.
class StFcsCollection : public StObject {
public:
    /// @brief Default constructor.
    StFcsCollection();
    /// @brief Destructor; frees all owned objects.
    ~StFcsCollection();
    
    void addHit(unsigned int det, StFcsHit*);            ///< Add a hit to the collection for detector @p det.
    StSPtrVecFcsHit& hits(unsigned int det);             ///< Return the hit list for detector @p det.
    const StSPtrVecFcsHit& hits(unsigned int det) const; ///< Return the hit list for detector @p det (const).
    unsigned int numberOfHits(unsigned int det) const;   ///< Return the number of hits for detector @p det.

    void addCluster(unsigned int det, StFcsCluster*);            ///< Add a cluster for detector @p det.
    StSPtrVecFcsCluster& clusters(unsigned int det);             ///< Return the cluster list for detector @p det.
    const StSPtrVecFcsCluster& clusters(unsigned int det) const; ///< Return the cluster list (const).
    unsigned int numberOfClusters(unsigned int det) const;       ///< Return the number of clusters for detector @p det.

    void addPoint(unsigned int det, StFcsPoint*);            ///< Add a photon point for detector @p det.
    StSPtrVecFcsPoint& points(unsigned int det);             ///< Return the photon point list for detector @p det.
    const StSPtrVecFcsPoint& points(unsigned int det) const; ///< Return the photon point list (const).
    unsigned int numberOfPoints(unsigned int det) const;     ///< Return the number of photon points for detector @p det.

    /// @brief Flag whether FCS data existed in the DAQ file.
    void setDataExist(int v) {mDataExist=v;}
    int isDataExist() {return mDataExist;} ///< Return 1 if FCS data existed in the DAQ file.

    /// @brief Return the packed FCS reconstruction flag word.
    int fcsReconstructionFlag()      const;
    /// @brief Set the packed FCS reconstruction flag word.
    void setFcsReconstructionFlag(int v);

    /// @brief Print collection summary.
    void print(int option=1);
    
private:
    StSPtrVecFcsHit     mHits[kFcsNDet+1];   ///< Hit arrays per sub-detector (+1 spare for empty channel).
    StSPtrVecFcsCluster mClusters[kFcsNDet]; ///< Cluster arrays per sub-detector.
    StSPtrVecFcsPoint   mPoints[kFcsNDet];   ///< Photon point arrays per sub-detector.

    Int_t mFcsReconstructionFlag=0;     ///< Packed FCS reconstruction flags (layout TBD).
    Int_t mDataExist=0;                 ///< 1 if FCS data existed in the DAQ file, 0 otherwise.

    ClassDef(StFcsCollection,2)
};

inline int StFcsCollection::fcsReconstructionFlag()      const {return mFcsReconstructionFlag;}
inline void StFcsCollection::setFcsReconstructionFlag(int v) {mFcsReconstructionFlag=v;}

#endif
